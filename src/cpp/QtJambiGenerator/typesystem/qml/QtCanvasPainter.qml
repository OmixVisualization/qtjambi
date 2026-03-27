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
    }
    ValueType{
        name: "QCanvasBoxShadow"
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::BoxShadow"
        Rejection{
            functionName: "type"
        }
    }
    ValueType{
        name: "QCanvasCustomBrush"
        polymorphicIdExpression: "%1->type() >= QCanvasBrush::BrushType::Custom"
        Rejection{
            functionName: "type"
        }
    }
    ValueType{
        name: "QCanvasGradientStop"
    }
    ValueType{
        name: "QCanvasGradient"
        polymorphicIdExpression: "false"
    }
    ValueType{
        name: "QCanvasGridPattern"
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::GridPattern"
        Rejection{
            functionName: "type"
        }
    }
    ValueType{
        name: "QCanvasImage"
    }
    ValueType{
        name: "QCanvasImagePattern"
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::ImagePattern"
        Rejection{
            functionName: "type"
        }
    }
    ValueType{
        name: "QCanvasBoxGradient"
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::BoxGradient"
        Rejection{
            functionName: "type"
        }
    }
    ValueType{
        name: "QCanvasLinearGradient"
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::LinearGradient"
        Rejection{
            functionName: "type"
        }
    }
    ValueType{
        name: "QCanvasRadialGradient"
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::RadialGradient"
        Rejection{
            functionName: "type"
        }
    }
    ValueType{
        name: "QCanvasConicalGradient"
        polymorphicIdExpression: "%1->type() == QCanvasBrush::BrushType::ConicalGradient"
        Rejection{
            functionName: "type"
        }
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
}
