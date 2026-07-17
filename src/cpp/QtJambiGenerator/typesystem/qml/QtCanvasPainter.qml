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
    packageName: "io.qt.canvaspainter"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtCanvasPainter"
    module: "qtjambi.canvaspainter"
    description: ""
    LoadTypeSystem{name: "QtQuick"}
    RequiredPackage{
        name: "io.qt.qml"
    }
    RequiredLibrary{
        name: "QtQml"
    }
    ValueType{
        name: "QCanvasBrush"
        isPolymorphicBase: true
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::Invalid"
        Rejection{enumName: "BrushType"}
        Rejection{functionName: "type"}
        Rejection{fieldName: "baseData"}
        until: [6, 11]
    }
    ValueType{
        name: "QCanvasBoxShadow"
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::BoxShadow"
        Rejection{
            functionName: "type"
        }
        until: [6, 11]
    }
    ValueType{
        name: "QCanvasCustomBrush"
        polymorphicIdExpression: "%1->type() >= QCanvasBrush::BrushType::Custom"
        Rejection{
            functionName: "type"
        }
        until: [6, 11]
    }
    ObjectType{
        name: "QCanvasGradient"
        Rejection{fieldName: "m_stops"}
        Rejection{fieldName: "m_type"}
        Rejection{fieldName: "m_imageId"}
        Rejection{fieldName: "m_imageY"}
        Rejection{fieldName: "m_data"}
        Rejection{fieldName: "m_cachedBrush"}
        Rejection{functionName: "type"}
        polymorphicIdExpression: "false"
        until: [6, 11]
    }
    ValueType{
        name: "QCanvasGridPattern"
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::GridPattern"
        Rejection{
            functionName: "type"
        }
        until: [6, 11]
    }
    ValueType{
        name: "QCanvasImagePattern"
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::ImagePattern"
        Rejection{
            functionName: "type"
        }
        until: [6, 11]
    }
    ValueType{
        name: "QCanvasBoxGradient"
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::BoxGradient"
        Rejection{
            functionName: "type"
        }
        until: [6, 11]
    }
    ValueType{
        name: "QCanvasLinearGradient"
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::LinearGradient"
        Rejection{
            functionName: "type"
        }
        until: [6, 11]
    }
    ValueType{
        name: "QCanvasRadialGradient"
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::RadialGradient"
        Rejection{
            functionName: "type"
        }
        until: [6, 11]
    }
    ValueType{
        name: "QCanvasConicalGradient"
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::ConicalGradient"
        Rejection{
            functionName: "type"
        }
        until: [6, 11]
    }
    ValueType{
        name: "QCanvasBrush"
        EnumType{
            name: "BrushType"
            extensible: true
        }
        Rejection{fieldName: "baseData"}
        ModifyFunction{
            signature: "as()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "as<T>()const"
            Instantiation{
                Argument{
                    type: "QCanvasBoxShadow"
                }
                rename: "asBoxShadow"
                InjectCode{
                    Text{content: String.raw`
                        if(type() != QCanvasBrush.BrushType.BoxShadow)
                            throw new IllegalStateException("Brush is not BoxShadow");`}
                }
            }
            Instantiation{
                Argument{
                    type: "QCanvasCustomBrush"
                }
                rename: "asCustomBrush"
                InjectCode{
                    Text{content: String.raw`
                        if(type().ordinal() >= QCanvasBrush.BrushType.Custom.ordinal())
                            throw new IllegalStateException("Brush is not Custom");`}
                }
            }
            Instantiation{
                Argument{
                    type: "QCanvasGridPattern"
                }
                rename: "asGridPattern"
                InjectCode{
                    Text{content: String.raw`
                        if(type() != QCanvasBrush.BrushType.GridPattern)
                            throw new IllegalStateException("Brush is not GridPattern");`}
                }
            }
            Instantiation{
                Argument{
                    type: "QCanvasImagePattern"
                }
                rename: "asImagePattern"
                InjectCode{
                    Text{content: String.raw`
                        if(type() != QCanvasBrush.BrushType.ImagePattern)
                            throw new IllegalStateException("Brush is not ImagePattern");`}
                }
            }
            Instantiation{
                Argument{
                    type: "QCanvasBoxGradient"
                }
                rename: "asBoxGradient"
                InjectCode{
                    Text{content: String.raw`
                        if(type() != QCanvasBrush.BrushType.BoxGradient)
                            throw new IllegalStateException("Brush is not BoxGradient");`}
                }
            }
            Instantiation{
                Argument{
                    type: "QCanvasLinearGradient"
                }
                rename: "asLinearGradient"
                InjectCode{
                    Text{content: String.raw`
                        if(type() != QCanvasBrush.BrushType.LinearGradient)
                            throw new IllegalStateException("Brush is not LinearGradient");`}
                }
            }
            Instantiation{
                Argument{
                    type: "QCanvasRadialGradient"
                }
                rename: "asRadialGradient"
                InjectCode{
                    Text{content: String.raw`
                        if(type() != QCanvasBrush.BrushType.RadialGradient)
                            throw new IllegalStateException("Brush is not RadialGradient");`}
                }
            }
            Instantiation{
                Argument{
                    type: "QCanvasConicalGradient"
                }
                rename: "asConicalGradient"
                InjectCode{
                    Text{content: String.raw`
                        if(type() != QCanvasBrush.BrushType.ConicalGradient)
                            throw new IllegalStateException("Brush is not ConicalGradient");`}
                }
            }
        }
        InjectCode{
            Text{content: String.raw`
                /**
                 * <p>See <code>QCanvasBrush::<wbr/>as&lt;T&gt;()const</code></p>
                 * @return
                 */
                @QtUninvokable
                public final <T> @NonNull T as(Class<T> type){
                    if(type==QCanvasBoxGradient.class)
                        return type.cast(asBoxGradient());
                    if(type==QCanvasBoxShadow.class)
                        return type.cast(asBoxShadow());
                    if(type==QCanvasRadialGradient.class)
                        return type.cast(asRadialGradient());
                    if(type==QCanvasLinearGradient.class)
                        return type.cast(asLinearGradient());
                    if(type==QCanvasConicalGradient.class)
                        return type.cast(asConicalGradient());
                    if(type==QCanvasImagePattern.class)
                        return type.cast(asImagePattern());
                    if(type==QCanvasGridPattern.class)
                        return type.cast(asGridPattern());
                    if(type==QCanvasCustomBrush.class)
                        return type.cast(asCustomBrush());
                    throw new IllegalArgumentException(type.getTypeName() + " is not a supported brush type");
                }`}
        }
        since: [6, 12]
    }
    ValueType{
        name: "QCanvasBoxShadow"
        ModifyFunction{
            signature: "operator QCanvasBrush()const"
            rename: "asBrush"
        }
        since: [6, 12]
    }
    ValueType{
        name: "QCanvasCustomBrush"
        ModifyFunction{
            signature: "operator QCanvasBrush()const"
            rename: "asBrush"
        }
        since: [6, 12]
    }
    ObjectType{
        name: "QCanvasGradient"
        ModifyFunction{
            signature: "QCanvasGradient()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QCanvasGradient(QCanvasGradient)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QCanvasGradient(QCanvasBrush::BrushType)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "~QCanvasGradient()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator QCanvasBrush()const"
            rename: "asBrush"
        }
        Rejection{fieldName: "m_stops"}
        Rejection{fieldName: "m_type"}
        Rejection{fieldName: "m_imageId"}
        Rejection{fieldName: "m_imageY"}
        Rejection{fieldName: "m_data"}
        Rejection{fieldName: "m_cachedBrush"}
        since: [6, 12]
    }
    ValueType{
        name: "QCanvasGridPattern"
        ModifyFunction{
            signature: "operator QCanvasBrush()const"
            rename: "asBrush"
        }
        since: [6, 12]
    }
    ValueType{
        name: "QCanvasImagePattern"
        ModifyFunction{
            signature: "operator QCanvasBrush()const"
            rename: "asBrush"
        }
        since: [6, 12]
    }
    ValueType{
        name: "QCanvasBoxGradient"
        ModifyFunction{
            signature: "operator QCanvasBrush()const"
            rename: "asBrush"
        }
        since: [6, 12]
    }
    ValueType{
        name: "QCanvasLinearGradient"
        ModifyFunction{
            signature: "operator QCanvasBrush()const"
            rename: "asBrush"
        }
        since: [6, 12]
    }
    ValueType{
        name: "QCanvasRadialGradient"
        ModifyFunction{
            signature: "operator QCanvasBrush()const"
            rename: "asBrush"
        }
        since: [6, 12]
    }
    ValueType{
        name: "QCanvasConicalGradient"
        ModifyFunction{
            signature: "operator QCanvasBrush()const"
            rename: "asBrush"
        }
        since: [6, 12]
    }
    ValueType{
        name: "QCanvasOffscreenCanvas"
        EnumType{
            name: "Flag"
        }
    }
    ObjectType{
        name: "QCanvasPainter"
        EnumType{
            name: "PathWinding"
        }
        EnumType{
            name: "PathConnection"
        }
        EnumType{
            name: "LineCap"
        }
        EnumType{
            name: "LineJoin"
        }
        EnumType{
            name: "TextAlign"
        }
        EnumType{
            name: "TextBaseline"
        }
        EnumType{
            name: "TextDirection"
        }
        EnumType{
            name: "CompositeOperation"
        }
        EnumType{
            name: "WrapMode"
        }
        EnumType{
            name: "ImageFlag"
        }
        EnumType{
            name: "RenderHint"
        }
        ModifyFunction{
            signature: "addImage(QRhiTexture*,QCanvasPainter::ImageFlags)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        InjectCode{
            target: CodeClass.Java
            Text{content: "Object __rcRhi;"}
        }
    }
    ObjectType{
        name: "QCanvasPainterFactory"
        InjectCode{
            target: CodeClass.Java
            Text{content: "private Object __rcRhi;"}
        }
        ModifyFunction{
            signature: "sharedInstance(QRhi*)"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "rhi"
                }
                Text{content: "if(__qt_return_value!=null){\n"+
                              "    __qt_return_value.__rcRhi = rhi;\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "painter()"
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "if(__qt_return_value!=null){\n"+
                              "    __qt_return_value.__rcRhi = __rcRhi;\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "create(QRhi*)"
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
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "rhi"
                }
                Text{content: "if(__qt_return_value!=null){\n"+
                              "    __qt_return_value.__rcRhi = rhi;\n"+
                              "}"}
            }
        }
    }
    ObjectType{
        name: "QCanvasPainterItem"
        ModifyFunction{
            signature: "createRenderer()"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "createItemRenderer()const"
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
        name: "QCanvasPainterItemRenderer"
    }
    ObjectType{
        name: "QCanvasPainterWidget"
    }
    ValueType{
        name: "QCanvasPath"
    }
    ValueType{
        name: "QCanvasGradientStop"
    }
    ValueType{
        name: "QCanvasImage"
    }
    ObjectType{
        name: "QCanvasRhiPaintDriver"
        EnumType{
            name: "BeginPaintFlag"
        }
        EnumType{
            name: "EndPaintFlag"
        }
    }
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QCanvasGradient' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QCanvasPainter::setStencilClip(const QVectorPath&) -> void', unmatched parameter type 'const QVectorPath&'"}
}
