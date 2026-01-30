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
    packageName: "io.qt.qt3d.animation"
    defaultSuperClass: "QtObject"
    qtLibrary: "Qt3DAnimation"
    module: "qtjambi.qt3danimation"
    description: "The Qt 3D Animation modules provides a set of prebuilt elements to help you get started with Qt 3D."
    
    NamespaceType{
        name: "Qt3DAnimation"
        NamespacePrefix{
            namingPolicy: NamespacePrefix.Cut
        }
        generate: false
        Rejection{ className: "QClipBlendNodeCreatedChange" }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QAbstractAnimation"
        EnumType{ name: "AnimationType" }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QAbstractAnimationClip"
    }
    
    ObjectType{
        name: "Qt3DAnimation::QAbstractChannelMapping"
    }
    
    ObjectType{
        name: "Qt3DAnimation::QAbstractClipAnimator"
        EnumType{
            name: "Loops"
            forceInteger: true
        }
        ModifyFunction{
            signature: "setChannelMapper(Qt3DAnimation::QChannelMapper *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setClock(Qt3DAnimation::QClock *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QAbstractClipBlendNode"
    }
    
    ObjectType{
        name: "Qt3DAnimation::QAdditiveClipBlend"
        ModifyFunction{
            signature: "setBaseClip(Qt3DAnimation::QAbstractClipBlendNode *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setAdditiveClip(Qt3DAnimation::QAbstractClipBlendNode *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "additiveClipChanged(Qt3DAnimation::QAbstractClipBlendNode*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QAnimationAspect"
    }
    
    ObjectType{
        name: "Qt3DAnimation::QAnimationCallback"
        EnumType{ name: "Flag" }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QAnimationClip"
    }
    
    ValueType{
        name: "Qt3DAnimation::QAnimationClipData"
        Rejection{functionName: "rbegin"}
        Rejection{functionName: "rend"}
        Rejection{functionName: "cbegin"}
        Rejection{functionName: "cend"}
        Rejection{functionName: "crbegin"}
        Rejection{functionName: "crend"}
        ModifyFunction{
            signature: "begin() const"
            access: Modification.Protected
        }
        ModifyFunction{
            signature: "end() const"
            access: Modification.Protected
        }

        IteratorType{
            name: "iterator"
            isConst: true
        }

        IteratorType{
            name: "const_iterator"
            isConst: true
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QAnimationClipLoader"
        EnumType{name: "Status"}
    }
    
    ObjectType{
        name: "Qt3DAnimation::QAnimationController"
        ModifyFunction{
            signature: "setEntity(Qt3DCore::QEntity *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setAnimationGroups(const QVector<Qt3DAnimation::QAnimationGroup *>&)"
            InjectCode{
                position: Position.End
                Text{content: "if(__rcAnimationGroup!=null){\n"+
                              "    __rcAnimationGroup.clear();\n"+
                              "}else{\n"+
                              "    __rcAnimationGroup = new java.util.ArrayList<>();\n"+
                              "}\n"+
                              "__rcAnimationGroup.addAll(animationGroups);"}
            }
        }
        ModifyFunction{
            signature: "addAnimationGroup(Qt3DAnimation::QAnimationGroup*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcAnimationGroup"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeAnimationGroup(Qt3DAnimation::QAnimationGroup*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcAnimationGroup"
                    action: ReferenceCount.Take
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QAnimationGroup"
        ModifyFunction{
            signature: "setAnimations(const QVector<Qt3DAnimation::QAbstractAnimation *>&)"
            InjectCode{
                position: Position.End
                Text{content: "if(__rcAnimation!=null){\n"+
                              "    __rcAnimation.clear();\n"+
                              "}else{\n"+
                              "    __rcAnimation = new java.util.ArrayList<>();\n"+
                              "}\n"+
                              "__rcAnimation.addAll(animations);"}
            }
        }
        ModifyFunction{
            signature: "addAnimation(Qt3DAnimation::QAbstractAnimation*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcAnimation"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeAnimation(Qt3DAnimation::QAbstractAnimation*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcAnimation"
                    action: ReferenceCount.Take
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QBlendedClipAnimator"
        ModifyFunction{
            signature: "setBlendTree(Qt3DAnimation::QAbstractClipBlendNode *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QCallbackMapping"
        ModifyFunction{
            signature: "setCallback(int,Qt3DAnimation::QAnimationCallback*,Qt3DAnimation::QAnimationCallback::Flags)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcCallback"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ValueType{
        name: "Qt3DAnimation::QChannel"
        Rejection{functionName: "rbegin"}
        Rejection{functionName: "rend"}
        Rejection{functionName: "cbegin"}
        Rejection{functionName: "cend"}
        Rejection{functionName: "crbegin"}
        Rejection{functionName: "crend"}
        ModifyFunction{
            signature: "begin() const"
            access: Modification.Protected
        }
        ModifyFunction{
            signature: "end() const"
            access: Modification.Protected
        }

        IteratorType{
            name: "const_iterator"
        }
    }
    
    ValueType{
        name: "Qt3DAnimation::QChannelComponent"

        Rejection{functionName: "rbegin"}
        Rejection{functionName: "rend"}
        Rejection{functionName: "cbegin"}
        Rejection{functionName: "cend"}
        Rejection{functionName: "crbegin"}
        Rejection{functionName: "crend"}

        IteratorType{
            name: "const_iterator"
        }

        ModifyFunction{
            signature: "begin() const"
            access: Modification.Protected
        }
        ModifyFunction{
            signature: "end() const"
            access: Modification.Protected
        }
        ModifyFunction{
            signature: "insertKeyFrame(int,Qt3DAnimation::QKeyFrame)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QChannelMapper"
        ModifyFunction{
            signature: "addMapping(Qt3DAnimation::QAbstractChannelMapping*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeMapping(Qt3DAnimation::QAbstractChannelMapping*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QChannelMapping"
        ModifyFunction{
            signature: "setTarget(Qt3DCore::QNode*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QClipAnimator"
        ModifyFunction{
            signature: "setClip(Qt3DAnimation::QAbstractAnimationClip*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QClipBlendNodeCreatedChangeBase"
    }
    
    ObjectType{
        name: "Qt3DAnimation::QClipBlendValue"
        ModifyFunction{
            signature: "setClip(Qt3DAnimation::QAbstractAnimationClip*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QClock"
    }
    
    ObjectType{
        name: "Qt3DAnimation::QKeyFrame"
        EnumType{
            name: "InterpolationType"
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QKeyframeAnimation"
        EnumType{
            name: "RepeatMode"
        }
        ModifyFunction{
            signature: "setKeyframes(const QVector<Qt3DCore::QTransform*>&)"
            InjectCode{
                position: Position.End
                Text{content: "if(__rcKeyframe!=null){\n"+
                              "    __rcKeyframe.clear();\n"+
                              "}else{\n"+
                              "    __rcKeyframe = new java.util.ArrayList<>();\n"+
                              "}\n"+
                              "__rcKeyframe.addAll(keyframes);"}
            }
        }
        ModifyFunction{
            signature: "addKeyframe(Qt3DCore::QTransform*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcKeyframe"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeKeyframe(Qt3DCore::QTransform*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcKeyframe"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "setTarget(Qt3DCore::QTransform*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcTarget"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QLerpClipBlend"
        ModifyFunction{
            signature: "setStartClip(Qt3DAnimation::QAbstractClipBlendNode*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setEndClip(Qt3DAnimation::QAbstractClipBlendNode*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    EnumType{
        name: "Qt3DAnimation::QMorphingAnimation::Method"
    }
    
    ObjectType{
        name: "Qt3DAnimation::QMorphingAnimation"
        ModifyFunction{
            signature: "setMorphTargets(const QVector<Qt3DAnimation::QMorphTarget*>&)"
            InjectCode{
                position: Position.End
                Text{content: "if(__rcMorphTarget!=null){\n"+
                              "    __rcMorphTarget.clear();\n"+
                              "}else{\n"+
                              "    __rcMorphTarget = new java.util.ArrayList<>();\n"+
                              "}\n"+
                              "__rcMorphTarget.addAll(targets);"}
            }
        }
        ModifyFunction{
            signature: "addMorphTarget(Qt3DAnimation::QMorphTarget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMorphTarget"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeMorphTarget(Qt3DAnimation::QMorphTarget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMorphTarget"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "setTarget(Qt3DRender::QGeometryRenderer*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcTarget"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QMorphTarget"
        ModifyFunction{
            signature: "fromGeometry(Qt3DCore::QGeometry*,QStringList)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "setAttributes(const QList<Qt3DCore::QAttribute*>&)"
            InjectCode{
                position: Position.End
                Text{content: "if(__rcAttribute!=null){\n"+
                              "    __rcAttribute.clear();\n"+
                              "}else{\n"+
                              "    __rcAttribute = new java.util.ArrayList<>();\n"+
                              "}\n"+
                              "__rcAttribute.addAll(attributes);"}
            }
        }
        ModifyFunction{
            signature: "addAttribute(Qt3DCore::QAttribute*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcAttribute"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeAttribute(Qt3DCore::QAttribute*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcAttribute"
                    action: ReferenceCount.Take
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QSkeletonMapping"
        ModifyFunction{
            signature: "setSkeleton(Qt3DCore::QAbstractSkeleton *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DAnimation::QVertexBlendAnimation"
        ModifyFunction{
            signature: "setMorphTargets(const QVector<Qt3DAnimation::QMorphTarget*>&)"
            InjectCode{
                position: Position.End
                Text{content: "if(__rcMorphTarget!=null){\n"+
                              "    __rcMorphTarget.clear();\n"+
                              "}else{\n"+
                              "    __rcMorphTarget = new java.util.ArrayList<>();\n"+
                              "}\n"+
                              "__rcMorphTarget.addAll(targets);"}
            }
        }
        ModifyFunction{
            signature: "addMorphTarget(Qt3DAnimation::QMorphTarget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMorphTarget"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeMorphTarget(Qt3DAnimation::QMorphTarget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMorphTarget"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "setTarget(Qt3DRender::QGeometryRenderer*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcTarget"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QAbstractChannelMapping' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QAbstractAnimationClip' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type Qt3DAnimation::QAnimationClipData."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type Qt3DAnimation::QChannel."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type Qt3DAnimation::QChannelComponent."}
}
