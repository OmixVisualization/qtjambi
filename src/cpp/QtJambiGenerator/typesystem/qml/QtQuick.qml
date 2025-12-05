/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    defaultSuperClass: "QtObject"
    qtLibrary: "QtQuick"
    module: "qtjambi.quick"
    description: "A declarative framework for building highly dynamic applications with custom user interfaces."
    LoadTypeSystem{name: "QtGuiRhi"; since: 6.6}
    LoadTypeSystem{name: "QtQml"}
    RequiredLibrary{
        name: "QmlWorkerScript"
        mode: RequiredLibrary.ProvideOnly
    }
    RequiredLibrary{
        name: "QmlModels"
        mode: RequiredLibrary.ProvideOnly
    }
    RequiredLibrary{
        name: "QmlMeta"
        mode: RequiredLibrary.ProvideOnly
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
                    modifiedType: "java.lang.@Nullable Object"
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
                    codeClass: CodeClass.Native
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
        className: "PtrShaderCreateFunc"
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
    
    InterfaceType{
        name: "QSGRendererInterface"
        EnumType{
            name: "GraphicsApi"
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
            name: "Resource"
            since: [5, 8]
        }

        EnumType{
            name: "ShaderType"
            since: [5, 8]
        }

        EnumType{
            name: "RenderMode"
            since: 6
        }

        EnumType{
            name: "ShaderCompilationType"
            since: [5, 8]
        }

        EnumType{
            name: "ShaderSourceType"
            since: [5, 8]
        }
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
        }
        since: [5, 8]
    }
    
    NamespaceType{
        name: "QQuickOpenGLUtils"
        since: 6
    }
    
    ObjectType{
        name: "QQuickRenderControl"
        ExtraIncludes{
            Include{
                fileName: "QtGui/rhi/qrhi.h"
                location: Include.Global
            }
            since: 6.6
        }
        ModifyFunction{
            signature: "renderWindow(QPoint *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "renderWindowFor(QQuickWindow *, QPoint *)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QSGContextPlugin"
    }
    
    ObjectType{
        name: "QQuickCloseEvent"
        packageName: "io.qt.quick.internal"
        forceFinal: true
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
        EnumType{
            name: "Flag"
        }
        EnumType{
            name: "ItemChange"
        }
        EnumType{
            name: "TransformOrigin"
        }

        ObjectType{
            name: "UpdatePaintNodeData"
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

        ValueType{
            name: "ItemChangeData"
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

        ModifyFunction{
            signature: "childMouseEventFilter(QQuickItem*,QEvent*)"
            noExcept: true
            blockExceptions: true
            ModifyArgument{
                index: 1
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: String.raw`
jobject %out;
try{
    %out = qtjambi_cast<jobject>(%env, %in);
    if(!%out && %in)
        return %class::childMouseEventFilter(%in, %2);
}catch(const JavaException&){
    return %class::childMouseEventFilter(%in, %2);
}
                        `}
                }
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: String.raw`
jobject %out;
try{
    %out = qtjambi_cast<jobject>(%env, %in);
}catch(const JavaException&){
    return %class::childMouseEventFilter(%1, %in);
}
                        `}
                }
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
            InjectCode{
                target: CodeClass.Java
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "java.util.Objects.requireNonNull(%1, \"Argument '%1': null not expected.\");"}
            }
        }
        ModifyFunction{
            signature: "focusOutEvent(QFocusEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "java.util.Objects.requireNonNull(%1, \"Argument '%1': null not expected.\");"}
            }
        }
        ModifyFunction{
            signature: "hoverEnterEvent(QHoverEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "java.util.Objects.requireNonNull(%1, \"Argument '%1': null not expected.\");"}
            }
        }
        ModifyFunction{
            signature: "hoverLeaveEvent(QHoverEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "java.util.Objects.requireNonNull(%1, \"Argument '%1': null not expected.\");"}
            }
        }
        ModifyFunction{
            signature: "hoverMoveEvent(QHoverEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "java.util.Objects.requireNonNull(%1, \"Argument '%1': null not expected.\");"}
            }
        }
        ModifyFunction{
            signature: "inputMethodEvent(QInputMethodEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "java.util.Objects.requireNonNull(%1, \"Argument '%1': null not expected.\");"}
            }
        }
        ModifyFunction{
            signature: "keyPressEvent(QKeyEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "java.util.Objects.requireNonNull(%1, \"Argument '%1': null not expected.\");"}
            }
        }
        ModifyFunction{
            signature: "keyReleaseEvent(QKeyEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "java.util.Objects.requireNonNull(%1, \"Argument '%1': null not expected.\");"}
            }
        }
        ModifyFunction{
            signature: "mouseDoubleClickEvent(QMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "java.util.Objects.requireNonNull(%1, \"Argument '%1': null not expected.\");"}
            }
        }
        ModifyFunction{
            signature: "mouseMoveEvent(QMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "java.util.Objects.requireNonNull(%1, \"Argument '%1': null not expected.\");"}
            }
        }
        ModifyFunction{
            signature: "mousePressEvent(QMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "java.util.Objects.requireNonNull(%1, \"Argument '%1': null not expected.\");"}
            }
        }
        ModifyFunction{
            signature: "mouseReleaseEvent(QMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "java.util.Objects.requireNonNull(%1, \"Argument '%1': null not expected.\");"}
            }
        }
        ModifyFunction{
            signature: "touchEvent(QTouchEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "java.util.Objects.requireNonNull(%1, \"Argument '%1': null not expected.\");"}
            }
        }
        ModifyFunction{
            signature: "wheelEvent(QWheelEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            InjectCode{
                target: CodeClass.Java
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "java.util.Objects.requireNonNull(%1, \"Argument '%1': null not expected.\");"}
            }
        }
        Import{
            template: "QuickItem"
        }
        ModifyFunction{
            signature: "grabToImage(QJSValue,QSize)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                noImplicitCalls: true
            }
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

        EnumType{
            name: "PerformanceHint"
        }

        EnumType{
            name: "RenderTarget"
        }
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
        EnumType{
            name: "Status"
        }
        ModifyFunction{
            signature: "setTextDocument(QTextDocument*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcTextDocument"
                    action: ReferenceCount.Set
                }
            }
            since: 6.7
        }
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
                    codeClass: CodeClass.Native
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

        EnumType{
            name: "ResizeMode"
        }
        EnumType{
            name: "Status"
        }
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
        Rejection{
            enumName: "NativeObjectType"
        }
        Rejection{
            functionName: "createTextureFromNativeObject"
        }

        EnumType{
            name: "CreateTextureOption"
        }

        EnumType{
            name: "SceneGraphError"
        }

        EnumType{
            name: "RenderStage"
        }

        EnumType{
            name: "TextRenderType"
            since: [5, 10]
        }

        ValueType{
            name: "GraphicsStateInfo"
            since: [5, 14]
        }
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
            signature: "createTextNode()const"
            threadAffinity: false
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            since: 6.7
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
            signature: "createTextureFromImage(QImage,QQuickWindow::CreateTextureOptions)const"
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
            signature: "createTextureFromRhiTexture(QRhiTexture*,QQuickWindow::CreateTextureOptions)const"
            threadAffinity: false
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
                ArgumentMap{index: 0; metaName: "%0"}
                ArgumentMap{index: 1; metaName: "%1"}
                Text{content: "if(%0!=null){\n"+
                              "    QtJambi_LibraryUtilities.internal.setCppOwnership(%1);\n"+
                              "    %0.destroyed.connect(()->QtJambi_LibraryUtilities.internal.setDefaultOwnership(%1));\n"+
                              "}"}
            }
            since: 6.6
        }
        ModifyFunction{
            signature: "createTextureFromId(uint,QSize,QQuickWindow::CreateTextureOptions)const"
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
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
                AddImplicitCall{type: "java.lang.@NonNull Runnable"}
            }
        }
        ModifyFunction{
            signature: "setGraphicsDevice(QQuickGraphicsDevice)"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{index: 1; metaName: "%1"}
                Text{content: "__rcDevice = %1==null ? null : %1.__rcDeviceContext;"}
            }
            since: 6
        }
        ModifyFunction{
            signature: "setRenderTarget(QQuickRenderTarget)"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{index: 1; metaName: "%1"}
                Text{content: "__rcRenderTarget = %1==null ? null : %1.__rcRenderTarget;"}
            }
            since: 6
        }
        InjectCode{
            Text{content: "private Object __rcDevice;\nprivate Object __rcRenderTarget;"}
            since: 6
        }
    }
    
    ObjectType{
        name: "QSGNode"

        EnumType{
            name: "DirtyStateBit"
        }

        EnumType{
            name: "Flag"
        }

        EnumType{
            name: "NodeType"
            generate: false
        }
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
            signature: "setRendererMatrix(const QMatrix4x4 *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcRendererMatrix"
                    action: ReferenceCount.Set
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
        EnumType{
            name: "TextureCoordinatesTransformFlag"
        }
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
                ArgumentMap{index: 1; metaName: "%1"}
                Text{content: "if (%1 != null && ownsTexture()) {\n"+
                              "    QtJambi_LibraryUtilities.internal.setCppOwnership(%1);\n"+
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
        name: "QSGTextNode"
        EnumType{
            name: "TextStyle"
        }
        EnumType{
            name: "RenderType"
        }
        ModifyFunction{
            signature: "addTextDocument(QPointF,QTextDocument*,int,int)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{index: 2; metaName: "%2"}
                Text{content: "addRC(%2);"}
            }
        }
        ModifyFunction{
            signature: "addTextLayout(QPointF,QTextLayout*,int,int,int,int)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{index: 2; metaName: "%2"}
                Text{content: "addRC(%2);"}
            }
        }
        ModifyFunction{
            signature: "clear()"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "__rcTextElements.clear();"}
            }
        }
        InjectCode{
            target: CodeClass.Java
            position: Position.End
            Text{content: "void addRC(Object obj){}"}
        }
        InjectCode{
            target: CodeClass.JavaConctreteWrapper
            position: Position.End
            Text{content: String.raw`
@QtPropertyMember(enabled=false)
private java.util.Collection<Object> __rcTextElements;

void addRC(Object obj){
    __rcTextElements.add(obj);
}
                `}
        }
        since: 6.7
    }
    
    ObjectType{
        name: "QSGRenderNode"
        ModifyFunction{
            signature: "flags() const"
            rename: "renderingFlags"
        }
        ModifyFunction{
            signature: "render(const QSGRenderNode::RenderState*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }

        ObjectType{
            name: "RenderState"
            ModifyFunction{
                signature: "get(const char *) const"
                remove: RemoveFlag.JavaAndNative
            }
        }

        EnumType{
            name: "StateFlag"
            since: [5, 8]
        }

        EnumType{
            name: "RenderingFlag"
            since: [5, 8]
        }
    }
    
    ObjectType{
        name: "QSGRootNode"
        ObjectType{
            name: "RenderState"
        }
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

        EnumType{
            name: "DataPattern"
        }

        EnumType{
            name: "Type"
        }

        EnumType{
            name: "DrawingMode"
        }

        EnumType{
            name: "AttributeType"
        }

        ValueType{
            name: "Point2D"
        }

        ValueType{
            name: "TexturedPoint2D"
        }

        ValueType{
            name: "Attribute"
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

        ValueType{
            name: "AttributeSet"
            InjectCode{
                target: CodeClass.Destructor
                position: Position.Position1
                Text{content: "delete[] %this->attributes;"}
            }
            CustomConstructor{
                type: CustomConstructor.Legacy
                Text{content: "if(copy){\n"+
                              "    QSGGeometry::AttributeSet* result = new(placement) QSGGeometry::AttributeSet{*copy};\n"+
                              "    QSGGeometry::Attribute* attributes;\n"+
                              "    result->attributes = attributes = copy->count>0 ? new QSGGeometry::Attribute[copy->count] : nullptr;\n"+
                              "    for(int i=0; i<copy->count; ++i){\n"+
                              "        attributes[i] = copy->attributes[i];\n"+
                              "    }\n"+
                              "    return result;\n"+
                              "}else{\n"+
                              "    return new(placement) QSGGeometry::AttributeSet{};\n"+
                              "}"}
            }
            CustomConstructor{
                type: CustomConstructor.Copy
                Text{content: "QSGGeometry::AttributeSet* result = new(placement) QSGGeometry::AttributeSet{*copy};\n"+
                              "QSGGeometry::Attribute* attributes;\n"+
                              "result->attributes = attributes = copy->count>0 ? new QSGGeometry::Attribute[copy->count] : nullptr;\n"+
                              "for(int i=0; i<copy->count; ++i){\n"+
                              "    attributes[i] = copy->attributes[i];\n"+
                              "}\n"}
            }
            CustomConstructor{
                type: CustomConstructor.Move
                Text{content: "new(placement) QSGGeometry::AttributeSet{std::move(*copy)};\n"+
                              "copy->count = 0;\n"+
                              "copy->attributes = nullptr;\n"+
                              "copy->stride = 0;\n"}
            }
            CustomDestructor{
                Text{content: "delete[] ptr->attributes;\n"+
                              "ptr->count = 0;\n"+
                              "ptr->stride = 0;\n"+
                              "ptr->attributes = nullptr;\n"+
                              "ptr->~AttributeSet();\n"}
            }

            ModifyFunction{
                signature: "AttributeSet(const QSGGeometry::AttributeSet&)"
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    Text{content: "QSGGeometry::Attribute* attributes;\n"+
                                  "__qt_this->attributes = attributes = __qt_%1.count>0 ? new QSGGeometry::Attribute[__qt_%1.count] : nullptr;\n"+
                                  "for(int i=0; i<__qt_%1.count; ++i){\n"+
                                  "    attributes[i] = __qt_%1.attributes[i];\n"+
                                  "}"}
                }
            }

            ModifyField{
                name: "attributes"
                ReplaceType{
                    modifiedType: "io.qt.quick.QSGGeometry$@NonNull Attribute @Nullable..."
                }
                ConversionRule{
                    codeClass: CodeClass.NativeGetter
                    Text{content: "%out = qtjambi_cast<jobjectArray>(%env, %scope, %in, __qt_this->count);"}
                }
                ConversionRule{
                    codeClass: CodeClass.NativeSetter
                    Text{content: String.raw`
__qt_this->count = %in ? %env->GetArrayLength(jobjectArray(%in)) : 0;
QSGGeometry::Attribute* %out = __qt_this->count>0 ? new QSGGeometry::Attribute[__qt_this->count] : nullptr;
for(jsize i=0; i<jsize(__qt_this->count); ++i){
    jobject element = %env->GetObjectArrayElement(jobjectArray(%in), i);
    JavaException::check(%env QTJAMBI_STACKTRACEINFO);
    %out[i] = qtjambi_cast<const QSGGeometry::Attribute&>(%env, element);
}`
                    }
                }
            }
            ModifyField{
                name: "count"
                write: false
            }
            ModifyField{
                name: "stride"
            }
            ModifyFunction{
                signature: "AttributeSet{int,int,const QSGGeometry::Attribute*}"
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "io.qt.quick.QSGGeometry$@NonNull Attribute @Nullable..."
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
jsize count0 = %in ? %env->GetArrayLength(%in) : 0;
QSGGeometry::Attribute* %out = count0>0 ? new QSGGeometry::Attribute[count0] : nullptr;
for(jsize i=0; i<count0; ++i){
    jobject element = %env->GetObjectArrayElement(%in, i);
    JavaException::check(%env QTJAMBI_STACKTRACEINFO);
    %out[i] = qtjambi_cast<const QSGGeometry::Attribute&>(%env, element);
}`
                        }
                    }
                }
                ModifyArgument{
                    index: 1
                    RemoveArgument{}
                }
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
            name: "ColoredPoint2D"
        }

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
                    Text{content: String.raw`
if(__qtjambi_constructor_options & QtJambiAPI::IsNativeConstruction)
    %in = Java::QtQuick::QSGGeometry$AttributeSet::clone(%env, %in);
const QSGGeometry::AttributeSet& %out = QtJambiAPI::checkedAddressOf(%env, QtJambiAPI::convertJavaObjectToNative<QSGGeometry::AttributeSet>(%env, %in));
if(__qtjambi_constructor_options & QtJambiAPI::IsNativeConstruction){
    if(%out.stride<=0)
        JavaException::raiseIllegalArgumentException(%env, "AttributeSet stride > 0 expected" QTJAMBI_STACKTRACEINFO);
    if(%out.count<=0)
        JavaException::raiseIllegalArgumentException(%env, "AttributeSet count > 0 expected" QTJAMBI_STACKTRACEINFO);
}
                        `}
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{index: 1; metaName: "%1"}
                Text{content: String.raw`
if(%1.stride()<=0)
    throw new IllegalArgumentException("AttributeSet stride > 0 expected");
if(%1.count()<=0)
    throw new IllegalArgumentException("AttributeSet count > 0 expected");
%1 = %1.clone();`
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
        ModifyFunction{
            signature: "indexData()"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.nio.@Nullable Buffer"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "switch(__qt_this->sizeOfIndex()){\n"+
                                  "case 4:\n"+
                                  "    %out = DataJBuffer(%env, reinterpret_cast<qint32*>(%in), __qt_this->indexCount()).take();\n"+
                                  "    break;\n"+
                                  "case 2:\n"+
                                  "    %out = DataJBuffer(%env, reinterpret_cast<qint16*>(%in), __qt_this->indexCount()).take();\n"+
                                  "    break;\n"+
                                  "default:\n"+
                                  "    break;\n"+
                                  "}"}
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
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
                AsArray{
                    lengthExpression: "__qt_this->attributeCount()"
                }
            }
        }
        ModifyFunction{
            signature: "QSGGeometry(QSGGeometry)"
            remove: RemoveFlag.All
            until: 5
        }
    }
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'QSGGeometry(QSGGeometry)' for function modification in 'QSGGeometry' not found.*"; until: 5}
    
    ObjectType{
        name: "QSGMaterial"

        EnumType{
            name: "Flag"
            RejectEnumValue{
                name: "CustomCompileStep"
                since: [6, 3]
            }
        }
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
        Rejection{
            functionName: "resourceUpdateBatch"
        }
        Rejection{
            functionName: "setShader"
        }
        EnumType{
            name: "Flag"
            since: 6
        }

        EnumType{
            name: "Stage"
            since: 6
        }

        ValueType{
            name: "RenderState"
            EnumType{
                name: "DirtyState"
            }
            Rejection{
                functionName: "rhi"
                until: 6.5
            }
            ModifyFunction{
                signature: "uniformData()"
                remove: RemoveFlag.All
                since: 6
            }
        }

        ValueType{
            name: "GraphicsPipelineState"
            EnumType{
                name: "BlendFactor"
            }
            EnumType{
                name: "PolygonMode"
            }
            EnumType{
                name: "ColorMaskComponent"
            }
            EnumType{
                name: "CullMode"
            }
            since: 6
        }
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
                    modifiedType: "java.lang.@Nullable String @Nullable[]"
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
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
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
            }
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
                AsArray{
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
        Rejection{
            fieldName: "m_texture"
        }
        Rejection{
            fieldName: "m_mipmap_filtering"
        }
        Rejection{
            fieldName: "m_filtering"
        }
        Rejection{
            fieldName: "m_horizontal_wrap"
        }
        Rejection{
            fieldName: "m_vertical_wrap"
        }
        Rejection{
            fieldName: "m_anisotropy_level"
        }
        Rejection{
            fieldName: "m_reserved"
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

    Rejection{
        className: "QSGTexturePrivate"
    }

    ObjectType{
        name: "QSGTexture"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/CoreAPI"
                location: Include.Global
            }
        }

        Rejection{
            functionName: "updateRhiTexture"
        }
        Rejection{
            functionName: "setWorkResourceUpdateBatch"
        }
        Rejection{
            functionName: "rhiTexture"
        }
        Rejection{
            functionName: "removedFromAtlas"
        }
        Rejection{
            functionName: "commitTextureOperations"
        }
        Rejection{
            className: "NativeTexture"
        }
        Rejection{
            functionName: "nativeTexture"
        }
        EnumType{
            name: "Filtering"
        }
        EnumType{
            name: "WrapMode"
        }
        EnumType{
            name: "AnisotropyLevel"
            since: [5, 9]
        }
        ModifyFunction{
            signature: "resolveInterface(const char *, int)const"
            rename: "nativeInterface"
            access: Modification.Public
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "<QNativeInterface extends io.qt.@Nullable QtObjectInterface> QNativeInterface"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = QtJambiAPI::convertNativeToJavaObjectAsWrapper(%env, %in, %1);"}
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
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
            }
        }
    }
    
    ObjectType{
        name: "QQuickImageProvider"
        ModifyFunction{
            signature: "requestImage(const QString &, QSize *, const QSize &)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "requestPixmap(const QString &, QSize *, const QSize &)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "requestTexture(const QString &, QSize *, const QSize &)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
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
        name: "QSGMaterialType"
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
            signature: "nodeChanged(QSGNode*,QSGNode::DirtyState)"
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
        until: 5
    }
    
    EnumType{
        name: "QSGAbstractRenderer::MatrixTransformFlag"
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
            signature: "createTextureFromImage(QImage,QSGEngine::CreateTextureOptions)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createTextureFromId(uint,QSize,QSGEngine::CreateTextureOptions)const"
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
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            since: 6
            Text{content: "QQuickGraphicsDevice qtjambi_QQuickGraphicsDevice_fromPhysicalDevice(JNIEnv *, jlong);\n"+
                          "QQuickGraphicsDevice qtjambi_QQuickGraphicsDevice_fromDeviceObjects(JNIEnv *, jlong, jlong, int, int);"}
        }
        ModifyFunction{
            signature: "fromPhysicalDevice(void*)"
            ppCondition: "QT_CONFIG(vulkan)"
            proxyCall: "qtjambi_QQuickGraphicsDevice_fromPhysicalDevice"
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
            since: 6
        }
        ModifyFunction{
            signature: "fromDeviceObjects(void*, void*, int, int)"
            ppCondition: "QT_CONFIG(vulkan)"
            proxyCall: "qtjambi_QQuickGraphicsDevice_fromDeviceObjects"
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
            since: 6
        }
        ModifyFunction{
            signature: "fromAdapter(quint32, qint32, int)"
            ppCondition: "defined(Q_OS_WIN)"
        }
        ModifyFunction{
            signature: "fromDeviceAndContext(void *, void *)"
            ppCondition: "defined(Q_OS_WIN)"
        }
        ModifyFunction{
            signature: "fromDeviceAndCommandQueue(MTLDevice *, MTLCommandQueue *)"
            ppCondition: "defined(Q_OS_MACOS) || defined(Q_OS_IOS)"
            since: 6
        }
        ModifyFunction{
            signature: "fromOpenGLContext(QOpenGLContext *)"
            ppCondition: "QT_CONFIG(opengl)"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{index: 0; metaName: "%0"}
                ArgumentMap{index: 1; metaName: "%1"}
                Text{content: "%0.__rcDeviceContext = %1;"}
            }
        }
        ModifyFunction{
            signature: "fromRhi(QRhi*)"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{index: 0; metaName: "%0"}
                ArgumentMap{index: 1; metaName: "%1"}
                Text{content: "%0.__rcDeviceContext = %1;"}
            }
            since: 6.6
        }
        ModifyFunction{
            signature: "fromRhiAdapter(QRhiAdapter*)"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{index: 0; metaName: "%0"}
                ArgumentMap{index: 1; metaName: "%1"}
                Text{content: "%0.__rcDeviceContext = %1;"}
            }
            since: [6,10]
        }
        ModifyFunction{
            signature: "QQuickGraphicsDevice(QQuickGraphicsDevice)"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{index: 0; metaName: "%0"}
                ArgumentMap{index: 1; metaName: "%1"}
                Text{content: "__rcDeviceContext = %1.__rcDeviceContext;"}
            }
        }
        ModifyFunction{
            signature: "operator=(QQuickGraphicsDevice)"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{index: 1; metaName: "%1"}
                Text{content: "__rcDeviceContext = %1.__rcDeviceContext;"}
            }
        }
        InjectCode{
            target: CodeClass.Java
            position: Position.Clone
            Text{content: "clone.__rcDeviceContext = this.__rcDeviceContext;"}
        }
        InjectCode{
            target: CodeClass.Java
            position: Position.End
            Text{content: "\nObject __rcDeviceContext;"}
        }
        since: 6
    }

    ObjectType{
        name: "VkImage"
        generate: false
        since: [6, 2]
    }

    ObjectType{
        name: "VkImageLayout"
        generate: false
        since: [6, 2]
    }
    
    ValueType{
        name: "QQuickRenderTarget"
        ExtraIncludes{
            Include{
                fileName: "QtGui/qvulkaninstance.h"
                location: Include.Global
                ckeckAvailability: true
            }
        }
        EnumType{
            name: "Flag"
            since: 6.8
        }

        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            since: 6
            Text{content: "QQuickRenderTarget qtjambi_QQuickRenderTarget_fromVulkanImage(JNIEnv *env, jlong image, jint layout, const QSize& pixelSize, int sampleCount);"}
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            since: [6, 4]
            Text{content: "QQuickRenderTarget qtjambi_QQuickRenderTarget_fromVulkanImage(JNIEnv *env, jlong image, jint layout, jint format, const QSize& pixelSize, int sampleCount);"}
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            since: [6, 8]
            Text{content: "QQuickRenderTarget qtjambi_QQuickRenderTarget_fromVulkanImage(JNIEnv *env, jlong image, jint layout, jint format, jint viewFormat, const QSize& pixelSize, int sampleCount, int arraySize, QQuickRenderTarget::Flags flags);"}
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
            signature: "fromD3D11Texture(void *, unsigned int, QSize, int, QQuickRenderTarget::Flags)"
            ppCondition: "defined(Q_OS_WIN)"
            since: [6, 8]
        }
        ModifyFunction{
            signature: "fromD3D12Texture(void *, int, uint, QSize, int)"
            ppCondition: "defined(Q_OS_WIN)"
            since: 6.6
        }
        ModifyFunction{
            signature: "fromD3D12Texture(void *, int, uint, uint, QSize, int, int, QQuickRenderTarget::Flags)"
            ppCondition: "defined(Q_OS_WIN)"
            since: 6.8
        }
        ModifyFunction{
            signature: "fromMetalTexture(MTLTexture *, QSize, int)"
            ppCondition: "defined(Q_OS_MACOS) || defined(Q_OS_IOS)"
            since: 6
        }
        ModifyFunction{
            signature: "fromMetalTexture(MTLTexture *, unsigned int, QSize, int)"
            ppCondition: "defined(Q_OS_MACOS) || defined(Q_OS_IOS)"
            since: [6, 4]
        }
        ModifyFunction{
            signature: "fromMetalTexture(MTLTexture *, unsigned int, unsigned int, QSize, int, int, QQuickRenderTarget::Flags)"
            ppCondition: "defined(Q_OS_MACOS) || defined(Q_OS_IOS)"
            since: [6, 8]
        }
        ModifyFunction{
            signature: "fromPaintDevice(QPaintDevice*)"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{index: 0; metaName: "%0"}
                ArgumentMap{index: 1; metaName: "%1"}
                Text{content: "%0.__rcRenderTarget = %1;"}
            }
            since: 6.4
        }
        ModifyFunction{
            signature: "fromRhiRenderTarget(QRhiRenderTarget*)"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{index: 0; metaName: "%0"}
                ArgumentMap{index: 1; metaName: "%1"}
                Text{content: "%0.__rcRenderTarget = %1;"}
            }
            since: 6.6
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
            since: 6
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
        ModifyFunction{
            signature: "fromVulkanImage(VkImage, VkImageLayout, VkFormat, VkFormat, QSize, int, int, QQuickRenderTarget::Flags)"
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
            ModifyArgument{
                index: 4
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
            since: [6, 8]
        }
        ModifyFunction{
            signature: "QQuickRenderTarget(QQuickRenderTarget)"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{index: 0; metaName: "%0"}
                ArgumentMap{index: 1; metaName: "%1"}
                Text{content: "__rcRenderTarget = %1.__rcRenderTarget;"}
            }
        }
        ModifyFunction{
            signature: "setDepthTexture(QRhiTexture*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: 6.8
        }
        ModifyFunction{
            signature: "operator=(QQuickRenderTarget)"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{index: 1; metaName: "%1"}
                Text{content: "__rcRenderTarget = %1.__rcRenderTarget;"}
            }
        }
        InjectCode{
            target: CodeClass.Java
            position: Position.Clone
            Text{content: "clone.__rcRenderTarget = this.__rcRenderTarget;"}
        }
        InjectCode{
            target: CodeClass.Java
            position: Position.End
            Text{content: "\nObject __rcRenderTarget;"}
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
        Rejection{
            functionName: "setShader"
        }

        EnumType{
            name: "Flag"
        }

        EnumType{
            name: "Stage"
        }

        ValueType{
            name: "RenderState"
            ModifyFunction{
                signature: "uniformData()"
                remove: RemoveFlag.All
            }
            Rejection{
                functionName: "resourceUpdateBatch"
            }
            Rejection{
                functionName: "rhi"
            }
        }

        ValueType{
            name: "GraphicsPipelineState"
            EnumType{
                name: "BlendFactor"
            }
            EnumType{
                name: "ColorMaskComponent"
            }
            EnumType{
                name: "CullMode"
            }
        }
        ModifyFunction{
            signature: "updateGraphicsPipelineState(QSGMaterialRhiShader::RenderState &, QSGMaterialRhiShader::GraphicsPipelineState *, QSGMaterial *, QSGMaterial *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
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
                AsArray{
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
    
    ValueType{
        name: "QQuickGraphicsConfiguration"
        since: 6
    }
        
    InterfaceType{
        name: "QNativeInterface::QSGOpenGLTexture"
        packageName: "io.qt.quick.nativeinterface"
        javaName: "QSGOpenGLTexture"
        ppCondition: "QT_CONFIG(opengl)"
        isNativeInterface: true
        generate: "no-shell"
        Rejection{
            className: "TypeInfo"
        }
        ModifyFunction{
            signature: "QSGOpenGLTexture()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "fromNative(GLuint,QQuickWindow*,QSize,QQuickWindow::CreateTextureOptions)"
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
            signature: "fromNativeExternalOES(GLuint,QQuickWindow*,QSize,QQuickWindow::CreateTextureOptions)"
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
        since: [6, 2]
    }
    
    InterfaceType{
        name: "QNativeInterface::QSGD3D11Texture"
        packageName: "io.qt.quick.nativeinterface"
        javaName: "QSGD3D11Texture"
        ppCondition: "defined(Q_OS_WIN)"
        isNativeInterface: true
        generate: "no-shell"
        Rejection{
            className: "TypeInfo"
        }
        ModifyFunction{
            signature: "QSGD3D11Texture()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "fromNative(void*,QQuickWindow*,QSize,QQuickWindow::CreateTextureOptions)"
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
        since: [6, 2]
    }

    InterfaceType{
        name: "QNativeInterface::QSGD3D12Texture"
        packageName: "io.qt.quick.nativeinterface"
        javaName: "QSGD3D12Texture"
        ppCondition: "defined(Q_OS_WIN)"
        isNativeInterface: true
        generate: "no-shell"
        Rejection{
            className: "TypeInfo"
        }
        ModifyFunction{
            signature: "QSGD3D12Texture()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "fromNative(void*,int,QQuickWindow*,QSize,QQuickWindow::CreateTextureOptions)"
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
        since: [6, 6]
    }
    
    InterfaceType{
        name: "QNativeInterface::QSGMetalTexture"
        packageName: "io.qt.quick.nativeinterface"
        javaName: "QSGMetalTexture"
        ppCondition: "defined(Q_OS_MACOS)"
        isNativeInterface: true
        generate: "no-shell"
        Rejection{
            className: "TypeInfo"
        }
        ModifyFunction{
            signature: "QSGMetalTexture()"
            remove: RemoveFlag.All
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "namespace QNativeInterface {\n"+
                          "    struct QSGMetalTexture{\n"+
                          "        QT_DECLARE_NATIVE_INTERFACE(QSGMetalTexture, 1, QSGTexture)\n"+
                          "    };\n"+
                          "}\n"+
                          "\n"+
                          "void* qtjambi_QSGMetalTexture_nativeTexture(JNIEnv *env, const void* ptr);\n"+
                          "QSGTexture * qtjambi_QSGMetalTexture_fromNative(JNIEnv *env, void* texture, QQuickWindow* window, const QSize& size, QQuickWindow::CreateTextureOptions options);\n"}
        }
        ModifyFunction{
            signature: "nativeTexture() const"
            proxyCall: "qtjambi_QSGMetalTexture_nativeTexture"
            ModifyArgument{
                index: 0
                replaceType: "io.qt.@Nullable QNativePointer"
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = QtJambiAPI::convertNativeToQNativePointer(%env, %in, QNativePointer::Type::Pointer, -1, 1);"}
                }
            }
        }
        ModifyFunction{
            signature: "fromNative(id<MTLTexture>, QQuickWindow *, QSize, QQuickWindow::CreateTextureOptions)"
            proxyCall: "qtjambi_QSGMetalTexture_fromNative"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            ModifyArgument{
                index: 1
                replaceType: "io.qt.@Nullable QNativePointer"
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "void* %out = QtJambiAPI::convertQNativePointerToNative(%env, %in);"}
                }
            }
        }
        since: [6, 2]
        until: 6.7
    }

    InterfaceType{
        name: "QNativeInterface::QSGMetalTexture"
        packageName: "io.qt.quick.nativeinterface"
        javaName: "QSGMetalTexture"
        ppCondition: "QT_CONFIG(metal)"
        isNativeInterface: true
        generate: "no-shell"
        Rejection{
            className: "TypeInfo"
        }
        ModifyFunction{
            signature: "QSGMetalTexture()"
            remove: RemoveFlag.All
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "void* qtjambi_QSGMetalTexture_nativeTexture(JNIEnv *env, const void* ptr);\n"+
                          "QSGTexture * qtjambi_QSGMetalTexture_fromNative(JNIEnv *env, void* texture, QQuickWindow* window, const QSize& size, QQuickWindow::CreateTextureOptions options);"}
        }
        ModifyFunction{
            signature: "nativeTexture() const"
            proxyCall: "qtjambi_QSGMetalTexture_nativeTexture"
            ModifyArgument{
                index: 0
                replaceType: "io.qt.@Nullable QNativePointer"
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = QtJambiAPI::convertNativeToQNativePointer(%env, %in, QNativePointer::Type::Pointer, -1, 1);"}
                }
            }
        }
        ModifyFunction{
            signature: "fromNative(id<MTLTexture>, QQuickWindow *, QSize, QQuickWindow::CreateTextureOptions)"
            proxyCall: "qtjambi_QSGMetalTexture_fromNative"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            ModifyArgument{
                index: 1
                replaceType: "io.qt.@Nullable QNativePointer"
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "void* %out = QtJambiAPI::convertQNativePointerToNative(%env, %in);"}
                }
            }
        }
        since: 6.8
    }
    
    InterfaceType{
        name: "QNativeInterface::QSGVulkanTexture"
        packageName: "io.qt.quick.nativeinterface"
        javaName: "QSGVulkanTexture"
        ppCondition: "QT_CONFIG(vulkan)"
        isNativeInterface: true
        generate: "no-shell"
        Rejection{
            className: "TypeInfo"
        }
        ExtraIncludes{
            Include{
                fileName: "QtGui/qvulkaninstance.h"
                location: Include.Global
                ckeckAvailability: true
            }
        }

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
            signature: "fromNative(VkImage, VkImageLayout, QQuickWindow *, QSize, QQuickWindow::CreateTextureOptions)"
            proxyCall: "qtjambi_QSGVulkanTexture_fromNative"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
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

    ObjectType{
        name: "QQuickRhiItem"
        EnumType{
            name: "TextureFormat"
        }
        ModifyFunction{
            signature: "createRenderer()"
            ModifyArgument{
                index: "return"
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
        since: 6.7
    }

    ObjectType{
        name: "QQuickRhiItemRenderer"
        since: 6.7
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *unmatched *type 'Vk*'"}
}
