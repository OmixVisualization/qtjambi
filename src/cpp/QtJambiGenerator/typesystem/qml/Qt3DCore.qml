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
    packageName: "io.qt.qt3d.core"
    defaultSuperClass: "QtObject"
    qtLibrary: "Qt3DCore"
    module: "qtjambi.qt3dcore"
    description: "The Qt 3D module contains functionality to support near-realtime simulation systems."
    RequiredLibrary{
        name: "QtConcurrent"
    }

    NamespacePrefix{
        prefix: "Qt3DCore"
        namespace: "Qt3DCore"
        namingPolicy: NamespacePrefix.Cut
    }
    
    NamespaceType{
        name: "Qt3DCore"

        Rejection{
            functionName: "functor_cast"
        }
        Rejection{
            functionName: "operator|"
        }
        Rejection{
            className: "QNodeCreatedChange"
        }
        Rejection{
            className: "FunctorType"
        }

        EnumType{
            name: "ChangeFlag"
        }
        ModifyFunction{
            signature: "qIdsForNodes<T>(T)"
            remove: RemoveFlag.All
        }
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
            signature: "operator <<(QDebug,Qt3DCore::QNodeId)"
            remove: RemoveFlag.All
        }
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
        className: "Qt3DCore::QAbstractFunctor"
        functionName: "functor_cast"
    }
    
    ObjectType{
        name: "Qt3DCore::QGeometry"

        Rejection{
            functionName: "boundingVolumeSpecifier"
        }
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
    }
    
    ObjectType{
        name: "Qt3DCore::QAttribute"
        ModifyFunction{
            signature: "setBuffer(Qt3DCore::QBuffer*)"
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
    }
    
    ObjectType{
        name: "Qt3DCore::QBuffer"
        EnumType{
            name: "BufferType"
        }
        EnumType{
            name: "UsageType"
        }
        EnumType{
            name: "AccessType"
        }
    }
    
    InterfaceType{
        name: "Qt3DCore::QAbstractFunctor"
        ModifyFunction{
            signature: "id()const"
            remove: RemoveFlag.JavaAndNative
            InjectCode{
                target: CodeClass.Shell
                Text{content: String.raw`
if(JniEnvironmentExceptionHandler %env{200}) {
    QTJAMBI_TRY {
        if(jobject %this = __shell()->getJavaObjectLocalRef(%env)){
            return qintptr(QtJambiAPI::getJavaObjectHashCode(%env, %env->GetObjectClass(%this)));
        }else{
            __shell()->warnForMethod("Qt3DCore::QAbstractFunctor::id() const");
        }
    } QTJAMBI_CATCH(const JavaException& exn){
        %env.handleException(exn, this, "Qt3DCore::QAbstractFunctor::id() const");
    } QTJAMBI_TRY_END
}`}
            }
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QBoundingVolume"
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
    }
    
    ObjectType{
        name: "Qt3DCore::QCoreAspect"
    }
    
    ObjectType{
        name: "Qt3DCore::QCoreSettings"
    }
    
    ObjectType{
        name: "Qt3DCore::QGeometryView"

        EnumType{
            name: "PrimitiveType"
        }
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
    }
    
    InterfaceType{
        name: "Qt3DCore::QSceneInterface"
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
    }
    
    ValueType{
        name: "Qt3DCore::QNodeId"
        ModifyFunction{
            signature: "operator bool() const"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QNode"

        EnumType{
            name: "PropertyTrackingMode"
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
    }
    
    ObjectType{
        name: "Qt3DCore::QAbstractSkeleton"
    }
    
    ObjectType{
        name: "Qt3DCore::QArmature"
        ModifyFunction{
            signature: "setSkeleton(Qt3DCore::QAbstractSkeleton*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QJoint"
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
    }
    
    ObjectType{
        name: "Qt3DCore::QComponent"
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
        ModifyFunction{
            signature: "componentsOfType<T>() const"
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
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambi3DCore.java"
                quoteAfterLine: "class QEntity__"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QAbstractCameraController"
        ModifyFunction{
            signature: "setCamera(Qt3DRender::QCamera*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QBackendNode"

        EnumType{
            name: "Mode"
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QBackendScenePropertyChange"
    }
    
    ObjectType{
        name: "Qt3DCore::QScene"
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

        EnumType{
            name: "RunMode"
        }
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
    
    ValueType{
        name: "Qt3DCore::QAspectFactory"
        ModifyFunction{
            signature: "functionHelper(QObject *)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QScenePropertyChange"
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
            signature: "setPropertyName(const char *)"
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                Text{content: "__qt_this->__qt_propertyNameReset(container);"}
            }
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QAbstractAspect"
        ExtraIncludes{
            Include{
                fileName: "Qt3DCore/QBackendNodeMapper"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "registerBackendType<Frontend>(const QSharedPointer<Qt3DCore::QBackendNodeMapper> &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "unregisterBackendType<Frontend>()"
            remove: RemoveFlag.All
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
    
    ObjectType{
        name: "Qt3DCore::QAbstractBuffer"
    }

    ObjectType{
        name: "Qt3DCore::QAspectJob"
    }
    
    ObjectType{
        name: "Qt3DCore::QNodeVisitor"
    }
    
    
    ObjectType{
        name: "Qt3DCore::QOpenGLInformationService"
    }
    
    ObjectType{
        name: "Qt3DCore::QAbstractServiceProvider"
    }
    
    ObjectType{
        name: "Qt3DCore::QSystemInformationService"
    }
    
    ObjectType{
        name: "Qt3DCore::QAbstractTransform"
    }
    
    ObjectType{
        name: "Qt3DCore::QLookAtTransform"
    }
    
    ObjectType{
        name: "Qt3DCore::QMatrixTransform"
    }
    
    ObjectType{
        name: "Qt3DCore::QRotateTransform"
    }
    
    ObjectType{
        name: "Qt3DCore::QScaleTransform"
    }
    
    ObjectType{
        name: "Qt3DCore::QTranslateTransform"
    }
    
    ValueType{
        name: "Qt3DCore::QNodeIdTypePair"
    }
    
    ObjectType{
        name: "Qt3DCore::QBackendNodeMapper"
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
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QSkeleton"
        ModifyFunction{
            signature: "setRootJoint(Qt3DCore::QJoint*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QSkeletonLoader"
        EnumType{
            name: "Status"
        }
    }
    
    ObjectType{
        name: "Qt3DCore::QTransform"
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'doClone() const' for function modification in '*' not found.*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature '*' for function modification in 'Qt3DCore' not found.*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QAbstractSkeleton' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'Qt3DCore::QAspectJob::postFrame*', unmatched parameter type 'Qt3DCore::QAspectManager*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'sceneChangeEvent(*)' in 'QCameraLens'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type Qt3DCore::QNodeIdTypePair."}
}
