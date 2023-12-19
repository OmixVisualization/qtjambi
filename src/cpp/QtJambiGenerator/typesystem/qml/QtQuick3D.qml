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
    packageName: "io.qt.quick3d"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtQuick3D"
    module: "qtjambi.quick3d"
    description: "Provides a high-level API for creating 3D content or UIs based on Qt Quick."
    LoadTypeSystem{name: "QtQml"}

    RequiredLibrary{
        name: "QtQuick3DUtils"
        mode: RequiredLibrary.Optional
        until: [6, 2]
    }
    
    RequiredLibrary{
        name: "QtQuick3DAssetImport"
        mode: RequiredLibrary.Optional
        until: [6, 2]
    }
    
    RequiredLibrary{
        name: "QtQuick3DRender"
        mode: RequiredLibrary.Optional
        until: 5
    }
    
    RequiredLibrary{
        name: "QtShaderTools"
        mode: RequiredLibrary.Optional
        since: 6
        until: [6, 2]
    }
    
    Rejection{
        className: ""
        functionName: "qml_register_types_QtQuick3D"
    }

    ObjectType{
        name: "QQuick3DExtensionHelpers"
        targetType: "final class"
        ModifyFunction{
            signature: "QQuick3DExtensionHelpers()"
            remove: RemoveFlag.All
        }
        since: 6.6
    }

    ObjectType{
        name: "QQuick3DRenderExtension"
        since: 6.6
    }
    
    ObjectType{
        name: "QQuick3D"
        targetType: "final class"
        ModifyFunction{
            signature: "QQuick3D()"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QQuick3DGeometry"
        ObjectType{
            name: "TargetAttribute"
            since: 6.6
        }

        EnumType{
            name: "PrimitiveType"
        }

        ValueType{
            name: "Attribute"

            EnumType{
                name: "Semantic"
                RejectEnumValue{
                    name: "TexCoord0Semantic"
                    since: 6
                }
            }

            EnumType{
                name: "ComponentType"
            }
        }
    }
    
    ObjectType{
        name: "QQuick3DInstancing"
        ModifyFunction{
            signature: "instanceBuffer(int*)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 0
                }
            }
        }
        ModifyFunction{
            signature: "getInstanceBuffer(int*)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 0
                }
            }
        }

        ValueType{
            name: "InstanceTableEntry"
        }
        since: [6, 1]
    }
    
    ObjectType{
        name: "QQuick3DTextureData"
        EnumType{
            name: "Format"
        }
        since: 6
    }

    Rejection{className: "QSSGRenderGraphObject"}
    
    ObjectType{
        name: "QQuick3DObject"
        EnumType{
            name: "ItemChange"
        }
        ObjectType{
            name: "ItemChangeData"
            generate: false
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "setParentItem(QQuick3DObject *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "itemChange(QQuick3DObject::ItemChange, const QQuick3DObject::ItemChangeData &)"
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
                                  "case QQuick3DObject::ItemChildAddedChange:      // value.item\n"+
                                  "case QQuick3DObject::ItemChildRemovedChange:    // value.item\n"+
                                  "case QQuick3DObject::ItemParentHasChanged:      // value.item\n"+
                                  "    {\n"+
                                  "        %out = qtjambi_cast<jobject>(%env, %in.item);\n"+
                                  "    }\n"+
                                  "    break;\n"+
                                  "case QQuick3DObject::ItemSceneChange:           // value.sceneManager\n"+
                                  "    {\n"+
                                  "        %out = nullptr;//qtjambi_cast<jobject>(%env, %in.sceneManager);\n"+
                                  "    }\n"+
                                  "    break;\n"+
                                  "case QQuick3DObject::ItemVisibleHasChanged:     // value.boolValue\n"+
                                  "case QQuick3DObject::ItemActiveFocusHasChanged: // value.boolValue\n"+
                                  "case QQuick3DObject::ItemAntialiasingHasChanged: // value.boolValue\n"+
                                  "case QQuick3DObject::ItemEnabledHasChanged:      // value.boolValue\n"+
                                  "    {\n"+
                                  "        %out = qtjambi_cast<jobject>(%env, %in.boolValue);\n"+
                                  "    }\n"+
                                  "    break;\n"+
                                  "case QQuick3DObject::ItemOpacityHasChanged:     // value.realValue\n"+
                                  "case QQuick3DObject::ItemRotationHasChanged:    // value.realValue\n"+
                                  "case QQuick3DObject::ItemDevicePixelRatioHasChanged: // value.realValue\n"+
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
                                  "QQuick3DObject::ItemChangeData %out(false);\n"+
                                  "switch(__qt_%1){\n"+
                                  "case QQuick3DObject::ItemChildAddedChange:      // value.item\n"+
                                  "case QQuick3DObject::ItemChildRemovedChange:    // value.item\n"+
                                  "case QQuick3DObject::ItemParentHasChanged:      // value.item\n"+
                                  "    {\n"+
                                  "        if(%in && !Java::QtQuick3D::QQuick3DObject::isInstanceOf(%env, %in)){\n"+
                                  "            JavaException::raiseIllegalArgumentException(%env, \"Object of type QQuick3DObject expected.\" QTJAMBI_STACKTRACEINFO);\n"+
                                  "        }\n"+
                                  "        %out.item = qtjambi_cast<QQuick3DObject*>(%env, %in);\n"+
                                  "    }\n"+
                                  "    break;\n"+
                                  "case QQuick3DObject::ItemSceneChange:           // value.window\n"+
                                  "    {\n"+
                                  "        %out.sceneManager = nullptr;\n"+
                                  "    }\n"+
                                  "    break;\n"+
                                  "case QQuick3DObject::ItemVisibleHasChanged:     // value.boolValue\n"+
                                  "case QQuick3DObject::ItemActiveFocusHasChanged: // value.boolValue\n"+
                                  "case QQuick3DObject::ItemAntialiasingHasChanged: // value.boolValue\n"+
                                  "case QQuick3DObject::ItemEnabledHasChanged:      // value.boolValue\n"+
                                  "    {\n"+
                                  "        if(%in && !Java::Runtime::Boolean::isInstanceOf(%env, %in)){\n"+
                                  "            JavaException::raiseIllegalArgumentException(%env, \"Boolean value expected.\" QTJAMBI_STACKTRACEINFO);\n"+
                                  "        }\n"+
                                  "        %out.boolValue = QtJambiAPI::fromJavaBooleanObject(%env, %in);\n"+
                                  "    }\n"+
                                  "break;\n"+
                                  "case QQuick3DObject::ItemOpacityHasChanged:     // value.realValue\n"+
                                  "case QQuick3DObject::ItemRotationHasChanged:    // value.realValue\n"+
                                  "case QQuick3DObject::ItemDevicePixelRatioHasChanged: // value.realValue\n"+
                                  "    {\n"+
                                  "        if(%in && !Java::Runtime::Number::isInstanceOf(%env, %in)){\n"+
                                  "            JavaException::raiseIllegalArgumentException(%env, \"Number value expected.\" QTJAMBI_STACKTRACEINFO);\n"+
                                  "        }\n"+
                                  "        %out.realValue = QtJambiAPI::fromJavaDoubleObject(%env, %in);\n"+
                                  "    }\n"+
                                  "    break;\n"+
                                  "default:\n"+
                                  "    break;\n"+
                                  "}"}
                }
            }
        }
        InjectCode{
            target: CodeClass.ShellDeclaration
            position: Position.End
            Text{content: "public: QSSGRenderGraphObject* updateSpatialNode(QSSGRenderGraphObject* n) override {return n;}"}
        }
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping*, unmatched *type 'QSSGRenderGraphObject*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping*unmatched*type '*QQuick3DSceneManager*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'QQuick3D()' for function modification in 'QQuick3D' not found. Possible candidates:"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Unimplementable pure virtual function: QQuick3DObject::updateSpatialNode*"}
}
