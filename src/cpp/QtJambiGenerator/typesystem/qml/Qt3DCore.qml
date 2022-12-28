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
    packageName: "io.qt.qt3d.core"
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "Qt3DCore"
    module: "qtjambi.qt3dcore"
    description: "The Qt 3D module contains functionality to support near-realtime simulation systems."
    RequiredLibrary{
        name: "QtConcurrent"
    }
    
    NamespaceType{
        name: "Qt3DCore"
        ModifyFunction{
            signature: "functorTypeId<T>()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "elementType(GLint)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "tupleSizeFromType(GLint)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "byteSizeFromType(GLint)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "intersects(Qt3DCore::Sphere,Qt3DCore::Sphere)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "intersects(Qt3DCore::QAxisAlignedBoundingBox,Qt3DCore::QAxisAlignedBoundingBox)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator ==(Qt3DCore::QOpenGLFilter,Qt3DCore::QOpenGLFilter)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator !=(Qt3DCore::QOpenGLFilter,Qt3DCore::QOpenGLFilter)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "qHash(Qt3DCore::QNodeId, unsigned int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator <<(QDataStream&,const Qt3DCore::QRay3D&)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator >>(QDataStream&,Qt3DCore::QRay3D&)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator <<(QDebug,Qt3DCore::QNodeId)"
            remove: RemoveFlag.All
        }
    }
    
    Rejection{
        className: "Qt3DCore::QNodeCreatedChange"
    }
    
    Rejection{
        className: "Qt3DCore"
        functionName: "functor_cast"
        since: 6
    }
    
    Rejection{
        className: "Qt3DCore"
        functionName: "operator|"
    }
    
    Rejection{
        className: "Qt3DCore::QGeometryView"
        functionName: "setGeometryFactory"
    }
    
    Rejection{
        className: "Qt3DCore::QGeometryView"
        functionName: "geometryFactory"
    }
    
    Rejection{
        className: "Qt3DCore::QScenePropertyChange"
        functionName: "operatornew"
    }
    
    Rejection{
        className: "Qt3DCore::QScenePropertyChange"
        functionName: "operatordelete"
    }
    
    Rejection{
        className: "QAspectThread"
    }
    
    Rejection{
        className: "QFrameAllocator"
    }
    
    Rejection{
        className: "QHandle"
    }
    
    Rejection{
        className: "QHandleManager"
    }
    
    Rejection{
        className: "Qt3DCore::FunctorType"
    }
    
    Rejection{
        className: "Qt3DCore::QGeometry"
        functionName: "boundingVolumeSpecifier"
    }
    
    Rejection{
        className: "Qt3DCore::QAbstractFunctor"
        functionName: "functor_cast"
        since: 6
    }
    
    ObjectType{
        name: "Qt3DCore::QGeometry"
        javaName: "QGeometry"
        ModifyFunction{
            signature: "addAttribute(Qt3DCore::QAttribute*)"
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
            signature: "removeAttribute(Qt3DCore::QAttribute*)"
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
            signature: "setBoundingVolumePositionAttribute(Qt3DCore::QAttribute*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcBoundingVolumePositionAttribute"
                    action: ReferenceCount.Set
                }
            }
        }
        since: 6
    }
    
    ObjectType{
        name: "Qt3DCore::QAttribute"
        javaName: "QAttribute"
        ModifyFunction{
            signature: "setBuffer(Qt3DCore::QBuffer*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: 6
    }
    
    EnumType{
        name: "Qt3DCore::QAttribute::AttributeType"
        since: 6
    }
    
    EnumType{
        name: "Qt3DCore::QAttribute::VertexBaseType"
        since: 6
    }
    
    ObjectType{
        name: "Qt3DCore::QBuffer"
        javaName: "QBuffer"
        since: 6
    }
    
    EnumType{
        name: "Qt3DCore::QBuffer::BufferType"
        since: 6
    }
    
    EnumType{
        name: "Qt3DCore::QBuffer::UsageType"
        since: 6
    }
    
    EnumType{
        name: "Qt3DCore::QBuffer::AccessType"
        since: 6
    }
    
    InterfaceType{
        name: "Qt3DCore::QAbstractFunctor"
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
                              "        __shell()->warnForMethod(\"Qt3DCore::QAbstractFunctor::id() const\");\n"+
                              "    }\n"+
                              "}"}
            }
        }
        since: 6
    }
    
    ObjectType{
        name: "Qt3DCore::QBoundingVolume"
        javaName: "QBoundingVolume"
        ModifyFunction{
            signature: "setView(Qt3DCore::QGeometryView*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcView"
                    action: ReferenceCount.Set
                }
            }
        }
        since: 6
    }
    
    ObjectType{
        name: "Qt3DCore::QCoreAspect"
        javaName: "QCoreAspect"
        since: 6
    }
    
    ObjectType{
        name: "Qt3DCore::QCoreSettings"
        javaName: "QCoreSettings"
        since: 6
    }
    
    EnumType{
        name: "Qt3DCore::QGeometryView::PrimitiveType"
        since: 6
    }
    
    ObjectType{
        name: "Qt3DCore::QGeometryView"
        javaName: "QGeometryView"
        ModifyFunction{
            signature: "setGeometry(Qt3DCore::QGeometry*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: 6
    }
    
    EnumType{
        name: "Qt3DCore::ChangeFlag"
        flags: "Qt3DCore::ChangeFlags"
    }
    
    EnumType{
        name: "Qt3DCore::QNode::PropertyTrackingMode"
        since: [5, 9]
    }
    
    InterfaceType{
        name: "Qt3DCore::QSceneInterface"
        javaName: "Qt3DCoreScene"
        ModifyFunction{
            signature: "addObservable(Qt3DCore::QObservableInterface*,Qt3DCore::QNodeId)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcObservables"
                    declareVariable: "io.qt.qt3d.core.Qt3DScene"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeObservable(Qt3DCore::QObservableInterface*,Qt3DCore::QNodeId)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcObservables"
                    declareVariable: "io.qt.qt3d.core.Qt3DScene"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "nodeIdFromObservable(Qt3DCore::QObservableInterface*)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setArbiter(Qt3DCore::QLockableObserverInterface*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcArbiter"
                    declareVariable: "io.qt.qt3d.core.Qt3DScene"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    
    InterfaceType{
        name: "Qt3DCore::QBackendNodeFactory"
        javaName: "QBackendNodeFactory"
        ModifyFunction{
            signature: "createBackendNode(const QSharedPointer<Qt3DCore::QNodeCreatedChangeBase>&) const"
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
    
    InterfaceType{
        name: "Qt3DCore::QAspectJobProviderInterface"
        javaName: "QAspectJobProvider"
    }
    
    ValueType{
        name: "Qt3DCore::QNodeId"
        javaName: "QNodeId"
        ModifyFunction{
            signature: "operator bool() const"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QNode"
        javaName: "QNode"
        ModifyFunction{
            signature: "event(QEvent *)"
            remove: RemoveFlag.All
            until: [5, 6]
        }
        ModifyFunction{
            signature: "doClone() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setParent(Qt3DCore::QNode*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "sendCommand(const QString &, const QVariant &, unsigned int)"
            ModifyArgument{
                index: 3
                ReplaceDefaultExpression{
                    expression: "0"
                }
            }
            since: [5, 10]
        }
        ModifyFunction{
            signature: "sendCommand(const QString &, const QVariant &, unsigned long long)"
            ModifyArgument{
                index: 3
                ReplaceDefaultExpression{
                    expression: "0"
                }
            }
            since: [5, 10]
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QAbstractSkeleton"
        javaName: "QAbstractSkeleton"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DCore::QArmature"
        javaName: "QArmature"
        ModifyFunction{
            signature: "setSkeleton(Qt3DCore::QAbstractSkeleton*)"
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
        name: "Qt3DCore::QJoint"
        javaName: "QJoint"
        ModifyFunction{
            signature: "addChildJoint(Qt3DCore::QJoint *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeChildJoint(Qt3DCore::QJoint *)"
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
        name: "Qt3DCore::QComponent"
        javaName: "QComponent"
        ModifyFunction{
            signature: "addedToEntity(Qt3DCore::QEntity*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removedFromEntity(Qt3DCore::QEntity*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QEntity"
        javaName: "QEntity"
        ModifyFunction{
            signature: "componentsOfType<T>()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "addComponent(Qt3DCore::QComponent*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeComponent(Qt3DCore::QComponent*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        InjectCode{
            target: CodeClass.Java
            until: 5
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambi3DCore.java"
                quoteAfterLine: "class QEntity_5_"
                quoteBeforeLine: "}// class"
            }
        }
        InjectCode{
            target: CodeClass.Java
            since: 6
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambi3DCore.java"
                quoteAfterLine: "class QEntity_6_"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QAbstractCameraController"
        javaName: "QAbstractCameraController"
        ModifyFunction{
            signature: "setCamera(Qt3DRender::QCamera*)"
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
        name: "Qt3DCore::QSceneChange::Priority"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DCore::QSceneChange::ObservableType"
        until: [5, 6]
    }
    
    EnumType{
        name: "Qt3DCore::QSceneChange::DeliveryFlag"
        flags: "Qt3DCore::QSceneChange::DeliveryFlags"
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QSceneChange"
        javaName: "QSceneChange"
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QNodeCommand"
        javaName: "QNodeCommand"
        until: 5
    }
    
    EnumType{
        name: "Qt3DCore::QBackendNode::Mode"
    }
    
    ObjectType{
        name: "Qt3DCore::QBackendNode"
        javaName: "QBackendNode"
        ModifyFunction{
            signature: "setFactory(const Qt3DCore::QBackendNodeFactory*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcBackendNodeFactory"
                    action: ReferenceCount.Set
                }
            }
            until: [5, 6]
        }
        ModifyFunction{
            signature: "setPeer(Qt3DCore::QNode*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcPeer"
                    action: ReferenceCount.Set
                }
            }
            until: [5, 6]
        }
        ModifyFunction{
            signature: "sendCommand(const QString &, const QVariant &, unsigned int)"
            ModifyArgument{
                index: 3
                ReplaceDefaultExpression{
                    expression: "0"
                }
            }
            since: [5, 10]
        }
        ModifyFunction{
            signature: "sendCommand(const QString &, const QVariant &, unsigned long long)"
            ModifyArgument{
                index: 3
                ReplaceDefaultExpression{
                    expression: "0"
                }
            }
            since: [5, 10]
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QBackendScenePropertyChange"
        javaName: "QBackendScenePropertyChange"
    }
    
    ValueType{
        name: "Qt3DCore::QRay3D"
        javaName: "QRay3D"
        ModifyFunction{
            signature: "transform(const QMatrix4x4 &)"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "void"
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QScene"
        javaName: "QScene"
        ModifyFunction{
            signature: "addObservable(Qt3DCore::QObservableInterface*,Qt3DCore::QNodeId)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcObservables"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "addObservable(Qt3DCore::QNode*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcObservables"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeObservable(Qt3DCore::QObservableInterface*,Qt3DCore::QNodeId)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcObservables"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "removeObservable(Qt3DCore::QNode*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcObservables"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "setArbiter(Qt3DCore::QLockableObserverInterface*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcArbiter"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QAspectEngine"
        javaName: "QAspectEngine"
        ModifyFunction{
            signature: "setRootEntity(QSharedPointer<Qt3DCore::QEntity>)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "registerAspect(Qt3DCore::QAbstractAspect *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "unregisterAspect(Qt3DCore::QAbstractAspect *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    EnumType{
        name: "Qt3DCore::QAspectEngine::RunMode"
        since: [5, 14]
    }
    
    ValueType{
        name: "Qt3DCore::QAspectFactory"
        javaName: "QAspectFactory"
        ModifyFunction{
            signature: "operator=(const Qt3DCore::QAspectFactory &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "functionHelper(QObject *)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QScenePropertyChange"
        javaName: "QScenePropertyChange"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QScopedPointer"
                location: Include.Global
            }
            Include{
                fileName: "QtCore/QByteArray"
                location: Include.Global
            }
        }
        InjectCode{
            target: CodeClass.ShellDeclaration
            position: Position.End
            Text{content: "public:\n"+
                          "    inline void __qt_propertyNameReset(QByteArray* pointer){\n"+
                          "        __qt_propertyName.reset(pointer);\n"+
                          "    }\n"+
                          "private:\n"+
                          "    QScopedPointer<QByteArray> __qt_propertyName;"}
        }
        ModifyFunction{
            signature: "propertyName()const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jstring>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "setPropertyName(const char *)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const char * %out = nullptr;\n"+
                                  "QByteArray* container = nullptr;\n"+
                                  "if(%in){\n"+
                                  "    container = new QByteArray(qtjambi_cast<QByteArray>(%env, %in));\n"+
                                  "    %out = container->constData();\n"+
                                  "}"}
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                Text{content: "__qt_this->__qt_propertyNameReset(container);"}
            }
        }
    }
    
    EnumType{
        name: "Qt3DCore::QAbstractAspect::AspectType"
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DCore::QAbstractAspect"
        javaName: "QAbstractAspect"
        ExtraIncludes{
            Include{
                fileName: "Qt3DCore/QBackendNodeMapper"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "registerBackendType<Frontend,supportsSyncing>(const QSharedPointer<Qt3DCore::QBackendNodeFunctor> &)"
            remove: RemoveFlag.All
            until: [5, 6]
        }
        ModifyFunction{
            signature: "registerBackendType<Frontend,supportsSyncing>(const QSharedPointer<Qt3DCore::QBackendNodeMapper> &)"
            remove: RemoveFlag.All
            since: [5, 7]
            until: 5
        }
        ModifyFunction{
            signature: "registerBackendType<Frontend>(const QSharedPointer<Qt3DCore::QBackendNodeMapper> &)"
            remove: RemoveFlag.All
            since: [5, 7]
        }
        ModifyFunction{
            signature: "unregisterBackendType<Frontend>()"
            remove: RemoveFlag.All
            since: [5, 9]
        }
        InjectCode{
            target: CodeClass.Java
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambi3DCore.java"
                quoteAfterLine: "class QAbstractAspect___java"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ValueType{
        name: "Qt3DCore::QAxisAlignedBoundingBox"
        javaName: "QAxisAlignedBoundingBox"
        ModifyFunction{
            signature: "transform(const QMatrix4x4 &)"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "void"
                }
            }
        }
    }
    
    EnumType{
        name: "Qt3DCore::QCameraLens::ProjectionType"
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DCore::QCameraLens"
        javaName: "QCameraLens"
        until: [5, 6]
    }
    
    ObjectType{
        name: "Qt3DCore::QAbstractBuffer"
        javaName: "QAbstractBuffer"
    }
    
    ObjectType{
        name: "Qt3DCore::QAspectJob"
        javaName: "QAspectJob"
    }
    
    ObjectType{
        name: "Qt3DCore::QNodeVisitor"
        javaName: "QNodeVisitor"
    }
    
    
    ObjectType{
        name: "Qt3DCore::QOpenGLInformationService"
        javaName: "QOpenGLInformationService"
    }
    
    ObjectType{
        name: "Qt3DCore::QAbstractServiceProvider"
        javaName: "QAbstractServiceProvider"
    }
    
    ObjectType{
        name: "Qt3DCore::QSystemInformationService"
        javaName: "QSystemInformationService"
    }
    
    ObjectType{
        name: "Qt3DCore::QAbstractTransform"
        javaName: "QAbstractTransform"
    }
    
    ObjectType{
        name: "Qt3DCore::QLookAtTransform"
        javaName: "QLookAtTransform"
    }
    
    ObjectType{
        name: "Qt3DCore::QMatrixTransform"
        javaName: "QMatrixTransform"
    }
    
    ObjectType{
        name: "Qt3DCore::QRotateTransform"
        javaName: "QRotateTransform"
    }
    
    ObjectType{
        name: "Qt3DCore::QScaleTransform"
        javaName: "QScaleTransform"
    }
    
    ObjectType{
        name: "Qt3DCore::QTranslateTransform"
        javaName: "QTranslateTransform"
    }
    
    ObjectType{
        name: "Qt3DCore::QComponentAddedChange"
        javaName: "QComponentAddedChange"
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QComponentRemovedChange"
        javaName: "QComponentRemovedChange"
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QDynamicPropertyUpdatedChange"
        javaName: "QDynamicPropertyUpdatedChange"
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QNodeCreatedChangeBase"
        javaName: "QNodeCreatedChange"
        since: [5, 7]
        until: 5
    }
    
    ValueType{
        name: "Qt3DCore::QNodeIdTypePair"
        javaName: "QNodeIdTypePair"
        since: [5, 8]
    }
    
    ObjectType{
        name: "Qt3DCore::QNodeDestroyedChange"
        javaName: "QNodeDestroyedChange"
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QPropertyNodeAddedChange"
        javaName: "QPropertyNodeAddedChange"
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QPropertyNodeRemovedChange"
        javaName: "QPropertyNodeRemovedChange"
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QPropertyUpdatedChange"
        javaName: "QPropertyUpdatedChange"
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QPropertyUpdatedChangeBase"
        javaName: "QPropertyUpdatedChangeBase"
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QPropertyValueAddedChange"
        javaName: "QPropertyValueAddedChange"
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QPropertyValueAddedChangeBase"
        javaName: "QPropertyValueAddedChangeBase"
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QPropertyValueRemovedChange"
        javaName: "QPropertyValueRemovedChange"
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QPropertyValueRemovedChangeBase"
        javaName: "QPropertyValueRemovedChangeBase"
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QStaticPropertyUpdatedChangeBase"
        javaName: "QStaticPropertyUpdatedChangeBase"
        ModifyFunction{
            signature: "propertyName() const"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jstring>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "setPropertyName(const char *)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray string1(qtjambi_cast<QByteArray>(%env, %in));\n"+
                                  "const char* %out = string1.constData();"}
                }
            }
        }
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QStaticPropertyValueAddedChangeBase"
        javaName: "QStaticPropertyValueAddedChangeBase"
        ModifyFunction{
            signature: "propertyName() const"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jstring>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "setPropertyName(const char *)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray string1(qtjambi_cast<QByteArray>(%env, %in));\n"+
                                  "const char* %out = string1.constData();"}
                }
            }
        }
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QStaticPropertyValueRemovedChangeBase"
        javaName: "QStaticPropertyValueRemovedChangeBase"
        ModifyFunction{
            signature: "propertyName() const"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jstring>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "setPropertyName(const char *)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray string1(qtjambi_cast<QByteArray>(%env, %in));\n"+
                                  "const char* %out = string1.constData();"}
                }
            }
        }
        since: [5, 7]
        until: 5
    }
    
    ObjectType{
        name: "Qt3DCore::QBackendNodeMapper"
        javaName: "QBackendNodeMapper"
        ModifyFunction{
            signature: "create(const QSharedPointer<Qt3DCore::QNodeCreatedChangeBase>&) const"
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
            until: 5
        }
        ModifyFunction{
            signature: "create(Qt3DCore::QNodeId) const"
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
            since: 6
        }
        since: [5, 7]
    }
    
    ObjectType{
        name: "Qt3DCore::QSkeleton"
        javaName: "QSkeleton"
        ModifyFunction{
            signature: "setRootJoint(Qt3DCore::QJoint*)"
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
        name: "Qt3DCore::QSkeletonLoader::Status"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DCore::QSkeletonLoader"
        javaName: "QSkeletonLoader"
        since: [5, 10]
    }
    
    ObjectType{
        name: "Qt3DCore::QTransform"
        javaName: "QTransform"
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'Qt3DCore::operator<<', unmatched return type 'QDebug'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'Qt3DCore::operator|', unmatched return type 'QIncompatibleFlag'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'Qt3DCore::operator|', unmatched return type 'QFlags<ChangeFlags::enum_type>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'doClone()const' for function modification in '*' not found.*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature '*' for function modification in 'Qt3DCore' not found.*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QAbstractSkeleton' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'sendCommand*' for function modification in 'Qt3DCore::QNode' not found. Possible candidates: *"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'sendCommand*' for function modification in 'Qt3DCore::QBackendNode' not found. Possible candidates: *"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'Qt3DCore::QAspectJob::postFrame', unmatched parameter type 'Qt3DCore::QAspectManager*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'sceneChangeEvent(*)' in 'QCameraLens'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type Qt3DCore::QNodeIdTypePair."}
}
