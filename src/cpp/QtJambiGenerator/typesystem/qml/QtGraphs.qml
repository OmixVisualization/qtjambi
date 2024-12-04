/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    packageName: "io.qt.graphs"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtGraphs"
    module: "qtjambi.graphs"
    description: "Provides functionality for visualizing data in 3D as bar, scatter, and surface graphs."
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

    Rejection{
        className: "QQuickGraphsItem"
        since: 6.7
    }
    Rejection{className: "QAbstract3DAxisPrivate"}
    Rejection{className: "QAbstract3DSeriesPrivate"}
    Rejection{className: "QAbstractDataProxyPrivate"}

    NamespaceType{
        name: "QtGraphs"
        ExtraIncludes{
            Include{
                fileName: "QtGraphs/qutils.h"
                location: Include.Global
            }
        }
    }
    
    GlobalFunction{
        signature: "qDefaultSurfaceFormat(bool)"
        targetType: "QtGraphs"
    }
    
    ObjectType{
        name: "Q3DBars"
        ModifyFunction{
            signature: "setColumnAxis(QCategory3DAxis *)"
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
            signature: "setRowAxis(QCategory3DAxis *)"
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
            signature: "setValueAxis(QValue3DAxis *)"
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
            signature: "addAxis(QAbstract3DAxis *)"
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
            signature: "releaseAxis(QAbstract3DAxis *)"
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
            signature: "setPrimarySeries(QBar3DSeries *)"
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
            signature: "addSeries(QBar3DSeries *)"
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
            signature: "removeSeries(QBar3DSeries *)"
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
            signature: "insertSeries(int, QBar3DSeries *)"
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
        name: "Q3DCamera"

        EnumType{
            name: "CameraPreset"
        }
        until: 6.6
    }
    
    ObjectType{
        name: "Q3DInputHandler"
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
        name: "Q3DLight"
        until: 6.6
    }
    
    ObjectType{
        name: "Q3DObject"
        until: 6.6
    }
    
    ObjectType{
        name: "Q3DScatter"
        ModifyFunction{
            signature: "setAxisX(QValue3DAxis *)"
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
            signature: "setAxisY(QValue3DAxis *)"
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
            signature: "setAxisZ(QValue3DAxis *)"
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
            signature: "addAxis(QValue3DAxis *)"
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
            signature: "addSeries(QScatter3DSeries *)"
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
            signature: "releaseAxis(QValue3DAxis *)"
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
            signature: "removeSeries(QScatter3DSeries *)"
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
        name: "Q3DScene"
        ModifyFunction{
            signature: "setActiveCamera(Q3DCamera *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "setActiveLight(Q3DLight *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 6.6
        }
    }
    
    ObjectType{
        name: "Q3DSurface"
        ModifyFunction{
            signature: "addAxis(QValue3DAxis *)"
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
            signature: "addSeries(QSurface3DSeries *)"
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
            signature: "removeSeries(QSurface3DSeries *)"
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
            signature: "setAxisX(QValue3DAxis *)"
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
            signature: "setAxisY(QValue3DAxis *)"
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
            signature: "setAxisZ(QValue3DAxis *)"
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
        name: "Q3DTheme"

        EnumType{
            name: "ColorStyle"
        }

        EnumType{
            name: "Theme"
        }
        until: 6.7
    }
    
    ObjectType{
        name: "QAbstract3DAxis"

        EnumType{
            name: "AxisOrientation"
        }

        EnumType{
            name: "AxisType"
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
        name: "QAbstract3DGraph"

        EnumType{
            name: "RenderingMode"
        }

        EnumType{
            name: "CameraPreset"
        }

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
            signature: "setLocale(QLocale)"
            rename: "setGraphsLocale"
        }
        ModifyFunction{
            signature: "locale()const"
            rename: "graphsLocale"
        }
        ModifyFunction{
            signature: "addCustomItem(QCustom3DItem *)"
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
            signature: "addInputHandler(QAbstract3DInputHandler *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 6.7
        }
        ModifyFunction{
            signature: "addTheme(Q3DTheme *)"
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
            signature: "releaseCustomItem(QCustom3DItem *)"
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
            signature: "releaseInputHandler(QAbstract3DInputHandler *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 6.7
        }
        ModifyFunction{
            signature: "releaseTheme(Q3DTheme *)"
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
            signature: "removeCustomItem(QCustom3DItem *)"
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
            signature: "setActiveInputHandler(QAbstract3DInputHandler *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 6.7
        }
        ModifyFunction{
            signature: "setActiveTheme(Q3DTheme *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        until: 6.7
    }
    
    ObjectType{
        name: "QAbstract3DInputHandler"

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
            signature: "setScene(Q3DScene *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcScene"
                    action: ReferenceCount.Set
                }
            }
        }
        until: 6.7
    }
    
    ObjectType{
        name: "QAbstract3DSeries"

        EnumType{
            name: "Mesh"
        }

        EnumType{
            name: "SeriesType"
        }
    }
    
    ObjectType{
        name: "QAbstractDataProxy"

        EnumType{
            name: "DataType"
        }
    }
    
    ObjectType{
        name: "QBar3DSeries"
        ModifyFunction{
            signature: "setDataProxy(QBarDataProxy *)"
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
        name: "QBarDataItem"
        noImplicitConstructors: true
    }
    
    ObjectType{
        name: "QBarDataProxy"
        ModifyFunction{
            signature: "addRow(QBarDataRow*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "addRow(QBarDataRow*,QString)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "insertRow(int,QBarDataRow*)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "insertRow(int,QBarDataRow*,QString)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "setRow(int,QBarDataRow*)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "setRow(int,QBarDataRow*,QString)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "resetArray()"
            InjectCode{
                position: Position.Beginning
                Text{content: "QtJambi_LibraryUtilities.internal.invalidateObject(__rcArray);"}
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "resetArray(QBarDataArray*)"
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
            until: 6.6
        }
        ModifyFunction{
            signature: "resetArray(QBarDataArray*,QStringList,QStringList)"
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
            until: 6.6
        }
        InjectCode{
            Text{content: "private QBarDataArray __rcArray;"}
            until: 6.6
        }
    }
    
    ValueType{
        name: "QBarDataRow"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
    }
    
    ValueType{
        name: "QBarDataArray"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
    }
    
    ObjectType{
        name: "QCategory3DAxis"
    }
    
    ObjectType{
        name: "QCustom3DItem"
        noImplicitConstructors: true
    }
    
    ObjectType{
        name: "QCustom3DLabel"
    }
    
    ObjectType{
        name: "QCustom3DVolume"
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
        name: "QHeightMapSurfaceDataProxy"
    }
    
    ObjectType{
        name: "QItemModelBarDataProxy"

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
        name: "QItemModelScatterDataProxy"
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
        name: "QItemModelSurfaceDataProxy"

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
        name: "QLogValue3DAxisFormatter"
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
        name: "QScatter3DSeries"
        ModifyFunction{
            signature: "setDataProxy(QScatterDataProxy *)"
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
        name: "QScatterDataItem"
        noImplicitConstructors: true
    }
    
    ObjectType{
        name: "QScatterDataProxy"
        ModifyFunction{
            signature: "resetArray(QScatterDataArray*)"
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
            until: 6.6
        }
        InjectCode{
            Text{content: "private QScatterDataArray __rcArray;"}
        }
    }
    
    ObjectType{
        name: "QScatterDataRow"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
    }
    
    ValueType{
        name: "QScatterDataArray"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
    }
    
    ObjectType{
        name: "QSurface3DSeries"

        EnumType{
            name: "DrawFlag"
        }
        ModifyFunction{
            signature: "setDataProxy(QSurfaceDataProxy *)"
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
        name: "QSurfaceDataItem"
        noImplicitConstructors: true
    }
    
    ObjectType{
        name: "QSurfaceDataProxy"
        ModifyFunction{
            signature: "addRow(QSurfaceDataRow*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "insertRow(int,QSurfaceDataRow*)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "setRow(int,QSurfaceDataRow*)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "resetArray(QSurfaceDataArray*)"
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
            until: 6.6
        }
        InjectCode{
            Text{content: "private QSurfaceDataArray __rcArray;"}
            until: 6.6
        }
    }
    
    ObjectType{
        name: "QSurfaceDataRow"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
    }
    
    ValueType{
        name: "QSurfaceDataArray"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
            since: 6.7
        }
    }
    
    ObjectType{
        name: "QTouch3DInputHandler"
        ModifyFunction{
            signature: "touchEvent(QTouchEvent *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QValue3DAxis"
        ModifyFunction{
            signature: "setFormatter(QValue3DAxisFormatter *)"
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
        name: "QValue3DAxisFormatter"
        ModifyFunction{
            signature: "setAxis(QValue3DAxis *)"
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
            signature: "gridPositions()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "labelPositions()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "labelStrings()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "subGridPositions()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
            until: 6.6
        }
    }

    ObjectType{
        name: "QSeriesTheme"
        EnumType{
            name: "SeriesColorTheme"
        }
        since: 6.7
        until: 6.7
    }

    ObjectType{
        name: "QAbstractAxis"
        EnumType{
            name: "AxisType"
        }
        Rejection{fieldName: "d_ptr"}
        ModifyFunction{
            signature: "setLabelDelegate(QQmlComponent*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcLabelComponent"
                    action: ReferenceCount.Set
                }
            }
            since: 6.8
        }
        since: 6.7
    }

    ObjectType{
        name: "QAbstractBarSeries"
        EnumType{
            name: "LabelsPosition"
        }
        ModifyFunction{
            signature: "remove(QBarSet *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcInsertedSets"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "take(QBarSet *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcInsertedSets"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "insert(int,QBarSet *)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcInsertedSets"
                    action: ReferenceCount.Add
                }
            }
        }
        since: 6.7
        until: 6.7
    }

    ObjectType{
        name: "QAbstractSeries"
        EnumType{
            name: "SeriesType"
        }
        ModifyFunction{
            signature: "setTheme(QSeriesTheme *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcTheme"
                    action: ReferenceCount.Set
                }
            }
            until: 6.7
        }
        Rejection{fieldName: "d_ptr"}
        Rejection{functionName: "graph"}
        Rejection{functionName: "setGraph"}
        since: 6.7
    }

    ObjectType{
        name: "QBarCategoryAxis"
        since: 6.7
    }

    ObjectType{
        name: "QBarSeries"
        EnumType{
            name: "LabelsPosition"
        }
        EnumType{
            name: "BarsType"
        }
        EnumType{
            name: "BarsOrientation"
        }
        ModifyFunction{
            signature: "append(QBarSet*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "append(QList<QBarSet*>)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insert(int,QBarSet*)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcInsertedSets"
                    action: ReferenceCount.Add
                }
            }
            until: 6.7
        }
        ModifyFunction{
            signature: "insert(qsizetype,QBarSet*)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcInsertedSets"
                    action: ReferenceCount.Add
                }
            }
            since: 6.8
        }
        ModifyFunction{
            signature: "replace(qsizetype,QBarSet*)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcInsertedSets"
                    action: ReferenceCount.Add
                }
            }
            since: 6.8
        }
        ModifyFunction{
            signature: "replace(QBarSet*,QBarSet*)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcInsertedSets"
                    action: ReferenceCount.Add
                }
            }
            since: 6.8
        }
        ModifyFunction{
            signature: "replace(QList<QBarSet*>)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcInsertedSets"
                    action: ReferenceCount.AddAll
                }
            }
            since: 6.8
        }
        ModifyFunction{
            signature: "remove(QBarSet*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcInsertedSets"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "take(QBarSet*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcInsertedSets"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "setAxisX(QAbstractAxis*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcAxisX"
                    action: ReferenceCount.Set
                }
            }
            until: 6.7
        }
        ModifyFunction{
            signature: "setAxisY(QAbstractAxis*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcAxisY"
                    action: ReferenceCount.Set
                }
            }
            until: 6.7
        }
        ModifyFunction{
            signature: "setBarDelegate(QQmlComponent*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcBarComponent"
                    action: ReferenceCount.Set
                }
            }
            since: 6.8
        }
        since: 6.7
    }

    ObjectType{
        name: "QBarSet"
        ModifyFunction{
            signature: "operator<<(qreal)"
            remove: RemoveFlag.All
        }
        since: 6.7
    }

    ObjectType{
        name: "QLegendData"
        since: 6.8
    }

    ObjectType{
        name: "QGraphsTheme"
        EnumType{
            name: "Theme"
        }
        EnumType{
            name: "ColorStyle"
        }
        ModifyFunction{
            signature: "dirtyBits()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        since: 6.8
    }

    ObjectType{
        name: "QGraphsThemeDirtyBitField"
        since: 6.8
    }

    ObjectType{
        name: "QGraphsLine"
        ModifyFunction{
            signature: "create(QJSValue)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: String.raw`
#if QT_VERSION == QT_VERSION_CHECK(6,8,0)
template<> QExplicitlySharedDataPointer<QGraphsLinePrivate>::~QExplicitlySharedDataPointer() {
    Q_ASSERT(!d);
}
#endif
`}
            until: 6.8
        }
        since: 6.8
    }

    ObjectType{
        name: "QQuickGraphsLineValueType"
        ModifyFunction{
            signature: "create(QJSValue)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
        }
        since: 6.8
    }

    NamespaceType{
        name: "QtGraphs3D"
        EnumType{
            name: "SelectionFlag"
        }
        EnumType{
            name: "ShadowQuality"
        }
        EnumType{
            name: "ElementType"
        }
        EnumType{
            name: "OptimizationHint"
        }
        EnumType{
            name: "RenderingMode"
        }
        EnumType{
            name: "CameraPreset"
        }
        ExtraIncludes{
            Include{
                fileName: "QtGraphs/qutils.h"
                location: Include.Global
            }
        }
        since: 6.8
    }

    ObjectType{
        name: "QBarModelMapper"
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
            access: Modification.NonFinal
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcModel"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSeries(QBarSeries *)"
            access: Modification.NonFinal
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcSeries"
                    action: ReferenceCount.Set
                }
            }
        }
        since: 6.8
    }

    ObjectType{
        name: "QXYModelMapper"
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
            access: Modification.NonFinal
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcModel"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSeries(QXYSeries *)"
            access: Modification.NonFinal
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcSeries"
                    action: ReferenceCount.Set
                }
            }
        }
        since: 6.8
    }

    ObjectType{
        name: "QPieModelMapper"
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
            access: Modification.NonFinal
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcModel"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSeries(QPieSeries *)"
            access: Modification.NonFinal
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcSeries"
                    action: ReferenceCount.Set
                }
            }
        }
        since: 6.8
    }

    ObjectType{
        name: "QDateTimeAxis"
        since: 6.8
    }

    ObjectType{
        name: "QValueAxis"
        since: 6.8
    }

    ObjectType{
        name: "QAreaSeries"
        ModifyFunction{
            signature: "setLowerSeries(QXYSeries *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcLowerSeries"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setUpperSeries(QXYSeries *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcUpperSeries"
                    action: ReferenceCount.Set
                }
            }
        }
        since: 6.8
    }

    ObjectType{
        name: "QLineSeries"
        since: 6.8
    }

    ObjectType{
        name: "QPieSeries"
        ModifyFunction{
            signature: "append(QPieSlice*)"
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
            signature: "append(QList<QPieSlice*>)"
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
            signature: "take(QPieSlice*)"
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
            signature: "remove(QPieSlice*)"
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
            signature: "insert(qsizetype,QPieSlice*)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "operator<<(QPieSlice*)"
            remove: RemoveFlag.All
        }
        since: 6.8
    }

    ObjectType{
        name: "QPieSlice"
        since: 6.8
    }

    ObjectType{
        name: "QScatterSeries"
        since: 6.8
    }

    ObjectType{
        name: "QSplineSeries"
        ModifyFunction{
            signature: "getControlPoints()"
            threadAffinity: true
            ModifyArgument{
                index: 0
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        since: 6.8
    }

    ObjectType{
        name: "QXYSeries"
        ModifyFunction{
            signature: "setPointDelegate(QQmlComponent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcPointDelegate"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "operator<<(QPointF)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator<<(QList<QPointF>)"
            remove: RemoveFlag.All
        }
        since: 6.8
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: horribly broken type ''"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QAbstractDataProxy' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QAbstract3DSeries' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QAbstract3DAxis' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function '*()' in 'Q3DTheme'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type *Q*DataArray."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'from*' for function modification in 'QQuick*' not found. Possible candidates: "}
}
