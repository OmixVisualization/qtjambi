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
    packageName: "io.qt.datavis"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtDataVisualization"
    module: "qtjambi.datavisualization"
    description: "UI Components for creating stunning 3D data visualizations."
    Rejection{
        className: "*"
        functionName: "dptr"
    }
    
    Rejection{
        className: "*"
        functionName: "dptrc"
    }
    
    Rejection{
        className: "*"
        fieldName: "d_ptr"
    }
    
    NamespacePrefix{
        prefix: "QtDataVisualization"
        namespace: "QtDataVisualization"
        namingPolicy: NamespacePrefix.Cut
        until: 5
    }
    
    NamespacePrefix{
        prefix: "QtDataVisualization"
        namespace: ""
        since: 6
    }
    
    NamespaceType{
        name: "QtDataVisualization"
        ExtraIncludes{
            Include{
                fileName: "QtDataVisualization/qutils.h"
                location: Include.Global
            }
        }
    }
    
    GlobalFunction{
        signature: "qDefaultSurfaceFormat(bool)"
        targetType: "QtDataVisualization"
        since: 6
    }
    
    ObjectType{
        name: "QtDataVisualization::QAbstract3DAxisPrivate"
        generate: false
    }
    
    ObjectType{
        name: "QtDataVisualization::QAbstractDataProxyPrivate"
        generate: false
    }
    
    ObjectType{
        name: "QtDataVisualization::QAbstract3DSeriesPrivate"
        generate: false
    }
    
    ObjectType{
        name: "QtDataVisualization::QAbstract3DGraphPrivate"
        generate: false
    }
    
    ObjectType{
        name: "QtDataVisualization::Q3DBars"
        ModifyFunction{
            signature: "setColumnAxis(QtDataVisualization::QCategory3DAxis *)"
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
            signature: "setRowAxis(QtDataVisualization::QCategory3DAxis *)"
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
            signature: "setValueAxis(QtDataVisualization::QValue3DAxis *)"
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
            signature: "addAxis(QtDataVisualization::QAbstract3DAxis *)"
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
            signature: "releaseAxis(QtDataVisualization::QAbstract3DAxis *)"
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
            signature: "setPrimarySeries(QtDataVisualization::QBar3DSeries *)"
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
            signature: "addSeries(QtDataVisualization::QBar3DSeries *)"
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
            signature: "removeSeries(QtDataVisualization::QBar3DSeries *)"
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
            signature: "insertSeries(int, QtDataVisualization::QBar3DSeries *)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::Q3DCamera"

        EnumType{
            name: "CameraPreset"
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::Q3DInputHandler"
        ModifyFunction{
            signature: "mouseMoveEvent(QMouseEvent *,QPoint)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mousePressEvent(QMouseEvent *,QPoint)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseReleaseEvent(QMouseEvent *,QPoint)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "wheelEvent(QWheelEvent *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::Q3DLight"
    }
    
    ObjectType{
        name: "QtDataVisualization::Q3DObject"
    }
    
    ObjectType{
        name: "QtDataVisualization::Q3DScatter"
        ModifyFunction{
            signature: "setAxisX(QtDataVisualization::QValue3DAxis *)"
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
            signature: "setAxisY(QtDataVisualization::QValue3DAxis *)"
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
            signature: "setAxisZ(QtDataVisualization::QValue3DAxis *)"
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
            signature: "addAxis(QtDataVisualization::QValue3DAxis *)"
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
            signature: "addSeries(QtDataVisualization::QScatter3DSeries *)"
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
            signature: "releaseAxis(QtDataVisualization::QValue3DAxis *)"
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
            signature: "removeSeries(QtDataVisualization::QScatter3DSeries *)"
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
        name: "QtDataVisualization::Q3DScene"
        ModifyFunction{
            signature: "setActiveCamera(QtDataVisualization::Q3DCamera *)"
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
            signature: "setActiveLight(QtDataVisualization::Q3DLight *)"
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
        name: "QtDataVisualization::Q3DSurface"
        ModifyFunction{
            signature: "addAxis(QtDataVisualization::QValue3DAxis *)"
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
            signature: "addSeries(QtDataVisualization::QSurface3DSeries *)"
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
            signature: "removeSeries(QtDataVisualization::QSurface3DSeries *)"
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
            signature: "setAxisX(QtDataVisualization::QValue3DAxis *)"
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
            signature: "setAxisY(QtDataVisualization::QValue3DAxis *)"
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
            signature: "setAxisZ(QtDataVisualization::QValue3DAxis *)"
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
        name: "QtDataVisualization::Q3DTheme"

        EnumType{
            name: "ColorStyle"
        }

        EnumType{
            name: "Theme"
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QAbstract3DAxis"

        EnumType{
            name: "AxisOrientation"
        }

        EnumType{
            name: "AxisType"
        }
        ModifyFunction{
            signature: "QAbstract3DAxis(QtDataVisualization::QAbstract3DAxisPrivate *, QObject *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setLabels(QStringList)"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "labels()const"
            access: Modification.NonFinal
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QAbstract3DGraph"

        EnumType{
            name: "ElementType"
        }

        EnumType{
            name: "ShadowQuality"
        }

        EnumType{
            name: "OptimizationHint"
        }

        EnumType{
            name: "SelectionFlag"
        }
        ModifyFunction{
            signature: "QAbstract3DGraph(QtDataVisualization::QAbstract3DGraphPrivate *,const QSurfaceFormat *, QWindow *)"
            remove: RemoveFlag.All
        }
        DelegateBaseClass{
            baseClass: "QOpenGLFunctions"
            delegate: "functions"
        }
        ModifyFunction{
            signature: "addCustomItem(QtDataVisualization::QCustom3DItem *)"
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
            signature: "addInputHandler(QtDataVisualization::QAbstract3DInputHandler *)"
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
            signature: "addTheme(QtDataVisualization::Q3DTheme *)"
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
            signature: "releaseCustomItem(QtDataVisualization::QCustom3DItem *)"
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
            signature: "releaseInputHandler(QtDataVisualization::QAbstract3DInputHandler *)"
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
            signature: "releaseTheme(QtDataVisualization::Q3DTheme *)"
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
            signature: "removeCustomItem(QtDataVisualization::QCustom3DItem *)"
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
            signature: "setActiveInputHandler(QtDataVisualization::QAbstract3DInputHandler *)"
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
            signature: "setActiveTheme(QtDataVisualization::Q3DTheme *)"
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
        name: "QtDataVisualization::QAbstract3DInputHandler"

        EnumType{
            name: "InputView"
        }
        ModifyFunction{
            signature: "mouseMoveEvent(QMouseEvent *,QPoint)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mousePressEvent(QMouseEvent *,QPoint)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseReleaseEvent(QMouseEvent *,QPoint)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseDoubleClickEvent(QMouseEvent *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "touchEvent(QTouchEvent *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "wheelEvent(QWheelEvent *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "setScene(QtDataVisualization::Q3DScene *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcScene"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QAbstract3DSeries"

        EnumType{
            name: "Mesh"
        }

        EnumType{
            name: "SeriesType"
        }
        ModifyFunction{
            signature: "QAbstract3DSeries(QtDataVisualization::QAbstract3DSeriesPrivate *, QObject *)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QAbstractDataProxy"

        EnumType{
            name: "DataType"
        }
        ModifyFunction{
            signature: "QAbstractDataProxy(QtDataVisualization::QAbstractDataProxyPrivate *, QObject *)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QBar3DSeries"
        ModifyFunction{
            signature: "setDataProxy(QtDataVisualization::QBarDataProxy *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(%1!=null){\n"+
                              "    if(%1==dataProxy()){\n"+
                              "        throw new IllegalArgumentException(\"Proxy is already assigned to this series.\");\n"+
                              "    }else if(%1.series()!=null){\n"+
                              "        throw new IllegalArgumentException(\"Proxy is already assigned to another series.\");\n"+
                              "    }\n"+
                              "}"}
            }
        }
    }
    
    ValueType{
        name: "QtDataVisualization::QBarDataItem"
        noImplicitConstructors: true
    }
    
    ObjectType{
        name: "QtDataVisualization::QBarDataProxy"
        ModifyFunction{
            signature: "addRow(QtDataVisualization::QBarDataRow*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "addRow(QtDataVisualization::QBarDataRow*,QString)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertRow(int,QtDataVisualization::QBarDataRow*)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertRow(int,QtDataVisualization::QBarDataRow*,QString)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setRow(int,QtDataVisualization::QBarDataRow*)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setRow(int,QtDataVisualization::QBarDataRow*,QString)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "resetArray(QtDataVisualization::QBarDataArray*)"
            InjectCode{
                position: Position.Beginning
                Text{content: "QtJambi_LibraryUtilities.internal.invalidateObject(__rcArray);"}
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "__rcArray = %1;\n"+
                              "QtJambi_LibraryUtilities.internal.registerDependentObject(__rcArray, this);"}
            }
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "resetArray(QtDataVisualization::QBarDataArray*,QStringList,QStringList)"
            InjectCode{
                position: Position.Beginning
                Text{content: "QtJambi_LibraryUtilities.internal.invalidateObject(__rcArray);"}
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "__rcArray = %1;\n"+
                              "QtJambi_LibraryUtilities.internal.registerDependentObject(__rcArray, this);"}
            }
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        InjectCode{
            Text{content: "private QBarDataArray __rcArray;"}
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QBarDataRow"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
    }
    
    ValueType{
        name: "QtDataVisualization::QBarDataArray"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QCategory3DAxis"
    }
    
    ObjectType{
        name: "QtDataVisualization::QCustom3DItem"
        noImplicitConstructors: true
    }
    
    ObjectType{
        name: "QtDataVisualization::QCustom3DLabel"
    }
    
    ObjectType{
        name: "QtDataVisualization::QCustom3DVolume"
        ModifyFunction{
            signature: "setSubTextureData(Qt::Axis,int,const uchar*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "createTextureData(QVector<QImage*>)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
            InjectCode{
                position: Position.Beginning
                Text{content: "QtJambi_LibraryUtilities.internal.invalidateObject(__rcTextureData);"}
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 0
                    metaName: "%0"
                }
                Text{content: "__rcTextureData = %0;\n"+
                              "QtJambi_LibraryUtilities.internal.registerDependentObject(__rcTextureData, this);"}
            }
        }
        InjectCode{
            Text{content: "private QtObject __rcTextureData;"}
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QHeightMapSurfaceDataProxy"
    }
    
    ObjectType{
        name: "QtDataVisualization::QItemModelBarDataProxy"

        EnumType{
            name: "MultiMatchBehavior"
        }
        ModifyFunction{
            signature: "setItemModel(QAbstractItemModel*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcItemModel"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QItemModelScatterDataProxy"
        ModifyFunction{
            signature: "setItemModel(QAbstractItemModel *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcItemModel"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QItemModelSurfaceDataProxy"

        EnumType{
            name: "MultiMatchBehavior"
        }
        ModifyFunction{
            signature: "setItemModel(QAbstractItemModel *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcItemModel"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QLogValue3DAxisFormatter"
        ModifyFunction{
            signature: "createNewInstance()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QScatter3DSeries"
        ModifyFunction{
            signature: "setDataProxy(QtDataVisualization::QScatterDataProxy *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(%1!=null){\n"+
                              "    if(%1==dataProxy()){\n"+
                              "        throw new IllegalArgumentException(\"Proxy is already assigned to this series.\");\n"+
                              "    }else if(%1.series()!=null){\n"+
                              "        throw new IllegalArgumentException(\"Proxy is already assigned to another series.\");\n"+
                              "    }\n"+
                              "}"}
            }
        }
    }
    
    ValueType{
        name: "QtDataVisualization::QScatterDataItem"
        noImplicitConstructors: true
    }
    
    ObjectType{
        name: "QtDataVisualization::QScatterDataProxy"
        ModifyFunction{
            signature: "resetArray(QtDataVisualization::QScatterDataArray*)"
            InjectCode{
                position: Position.Beginning
                Text{content: "QtJambi_LibraryUtilities.internal.invalidateObject(__rcArray);"}
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "__rcArray = %1;\n"+
                              "QtJambi_LibraryUtilities.internal.registerDependentObject(__rcArray, this);"}
            }
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        InjectCode{
            Text{content: "private QScatterDataArray __rcArray;"}
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QScatterDataRow"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
    }
    
    ValueType{
        name: "QtDataVisualization::QScatterDataArray"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QSurface3DSeries"

        EnumType{
            name: "DrawFlag"
        }
        ModifyFunction{
            signature: "setDataProxy(QtDataVisualization::QSurfaceDataProxy *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(%1!=null){\n"+
                              "    if(%1==dataProxy()){\n"+
                              "        throw new IllegalArgumentException(\"Proxy is already assigned to this series.\");\n"+
                              "    }else if(%1.series()!=null){\n"+
                              "        throw new IllegalArgumentException(\"Proxy is already assigned to another series.\");\n"+
                              "    }\n"+
                              "}"}
            }
        }
    }
    
    ValueType{
        name: "QtDataVisualization::QSurfaceDataItem"
        noImplicitConstructors: true
    }
    
    ObjectType{
        name: "QtDataVisualization::QSurfaceDataProxy"
        ModifyFunction{
            signature: "addRow(QtDataVisualization::QSurfaceDataRow*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertRow(int,QtDataVisualization::QSurfaceDataRow*)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setRow(int,QtDataVisualization::QSurfaceDataRow*)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "resetArray(QtDataVisualization::QSurfaceDataArray*)"
            InjectCode{
                position: Position.Beginning
                Text{content: "QtJambi_LibraryUtilities.internal.invalidateObject(__rcArray);"}
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "__rcArray = %1;\n"+
                              "QtJambi_LibraryUtilities.internal.registerDependentObject(__rcArray, this);"}
            }
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        InjectCode{
            Text{content: "private QSurfaceDataArray __rcArray;"}
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QSurfaceDataRow"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
    }
    
    ValueType{
        name: "QtDataVisualization::QSurfaceDataArray"
    }
    
    ObjectType{
        name: "QtDataVisualization::QTouch3DInputHandler"
        ModifyFunction{
            signature: "touchEvent(QTouchEvent *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QValue3DAxis"
        ModifyFunction{
            signature: "setFormatter(QtDataVisualization::QValue3DAxisFormatter *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcFormatter"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QtDataVisualization::QValue3DAxisFormatter"
        ModifyFunction{
            signature: "createNewInstance()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "gridPositions()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        ModifyFunction{
            signature: "labelPositions()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        ModifyFunction{
            signature: "labelStrings()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        ModifyFunction{
            signature: "subGridPositions()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
    }
    
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type *Q*DataArray."}
}
