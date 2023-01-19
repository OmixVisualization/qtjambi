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
    packageName: "io.qt.charts"
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtCharts"
    module: "qtjambi.charts"
    description: "UI Components for displaying visually pleasing charts, driven by static or dynamic data models."
    NamespacePrefix{
        prefix: "QtCharts"
        namespace: "QtCharts"
        namingPolicy: NamespacePrefix.Cut
        until: 5
    }
    
    NamespacePrefix{
        prefix: "QtCharts"
        namespace: ""
        since: 6
    }
    
    NamespaceType{
        name: "QtCharts"
        generate: false
        until: 5
    }
    
    EnumType{
        name: "QtCharts::QAbstractAxis::AxisType"
        flags: "QtCharts::QAbstractAxis::AxisTypes"
    }
    
    ObjectType{
        name: "QtCharts::QAbstractAxis"
    }
    
    EnumType{
        name: "QtCharts::QAbstractBarSeries::LabelsPosition"
    }
    
    ObjectType{
        name: "QtCharts::QAbstractBarSeries"
        ModifyFunction{
            signature: "append(QtCharts::QBarSet *)"
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
            signature: "remove(QtCharts::QBarSet *)"
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
            signature: "take(QtCharts::QBarSet *)"
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
            signature: "insert(int,QtCharts::QBarSet *)"
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
        ModifyFunction{
            signature: "append(QList<QtCharts::QBarSet*>)"
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
    
    EnumType{
        name: "QtCharts::QAbstractSeries::SeriesType"
    }
    
    ObjectType{
        name: "QtCharts::QAbstractSeries"
        ModifyFunction{
            signature: "attachAxis(QtCharts::QAbstractAxis *)"
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
            signature: "detachAxis(QtCharts::QAbstractAxis *)"
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
        name: "QtCharts::QAreaLegendMarker"
    }
    
    ObjectType{
        name: "QtCharts::QAreaSeries"
        ModifyFunction{
            signature: "QAreaSeries(QtCharts::QLineSeries *, QtCharts::QLineSeries *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcLowerSeries"
                    action: ReferenceCount.Set
                }
            }
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcUpperSeries"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setLowerSeries(QtCharts::QLineSeries *)"
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
            signature: "setUpperSeries(QtCharts::QLineSeries *)"
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
    }
    
    ObjectType{
        name: "QtCharts::QBarCategoryAxis"
    }
    
    ObjectType{
        name: "QtCharts::QBarLegendMarker"
    }
    
    ObjectType{
        name: "QtCharts::QBarSeries"
    }
    
    ObjectType{
        name: "QtCharts::QBarSet"
        ModifyFunction{
            signature: "operator<<(qreal)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator[](const int)const"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QtCharts::QBoxPlotLegendMarker"
    }
    
    ObjectType{
        name: "QtCharts::QBoxPlotSeries"
        ModifyFunction{
            signature: "append(QtCharts::QBoxSet *)"
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
            signature: "remove(QtCharts::QBoxSet *)"
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
            signature: "take(QtCharts::QBoxSet *)"
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
            signature: "insert(int,QtCharts::QBoxSet *)"
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
        ModifyFunction{
            signature: "append(QList<QtCharts::QBoxSet*>)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcInsertedSets"
                    action: ReferenceCount.AddAll
                }
            }
        }
    }
    
    EnumType{
        name: "QtCharts::QBoxSet::ValuePositions"
    }
    
    ObjectType{
        name: "QtCharts::QBoxSet"
        ModifyFunction{
            signature: "operator<<(qreal)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator[](const int)const"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QtCharts::QCandlestickLegendMarker"
    }
    
    ObjectType{
        name: "QtCharts::QCandlestickModelMapper"
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
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
            signature: "setSeries(QtCharts::QCandlestickSeries *)"
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
    }
    
    ObjectType{
        name: "QtCharts::QCandlestickSeries"
        ModifyFunction{
            signature: "append(QtCharts::QCandlestickSet *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "remove(QtCharts::QCandlestickSet *)"
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
            signature: "take(QtCharts::QCandlestickSet *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "insert(int,QtCharts::QCandlestickSet *)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "append(QList<QtCharts::QCandlestickSet*>)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "remove(QList<QtCharts::QCandlestickSet*>)"
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
        name: "QtCharts::QCandlestickSet"
    }
    
    EnumType{
        name: "QtCharts::QCategoryAxis::AxisType"
    }
    
    EnumType{
        name: "QtCharts::QCategoryAxis::AxisLabelsPosition"
    }
    
    ObjectType{
        name: "QtCharts::QCategoryAxis"
    }
    
    EnumType{
        name: "QtCharts::QChart::AnimationOption"
        flags: "QtCharts::QChart::AnimationOptions"
    }
    
    EnumType{
        name: "QtCharts::QChart::ChartTheme"
    }
    
    EnumType{
        name: "QtCharts::QChart::ChartType"
    }
    
    ObjectType{
        name: "QtCharts::QChart"
        ModifyFunction{
            signature: "scroll(qreal,qreal)"
            rename: "scrollBy"
        }
        ModifyFunction{
            signature: "axes(QFlags<Qt::Orientation>, QtCharts::QAbstractSeries *)const"
            ModifyArgument{
                index: 1
                RemoveDefaultExpression{
                }
            }
        }
        ModifyFunction{
            signature: "addAxis(QtCharts::QAbstractAxis *, QFlags<Qt::AlignmentFlag>)"
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
            signature: "removeAxis(QtCharts::QAbstractAxis *)"
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
            signature: "addSeries(QtCharts::QAbstractSeries *)"
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
            signature: "removeSeries(QtCharts::QAbstractSeries *)"
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
            signature: "setAxisX(QtCharts::QAbstractAxis*,QtCharts::QAbstractSeries*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setAxisY(QtCharts::QAbstractAxis*,QtCharts::QAbstractSeries*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiCharts.java"
                quoteAfterLine: "class QChart___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    EnumType{
        name: "QtCharts::QChartView::RubberBand"
        flags: "QtCharts::QChartView::RubberBands"
    }
    
    ObjectType{
        name: "QtCharts::QChartView"
        ModifyFunction{
            signature: "setChart(QtCharts::QChart *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                position: Position.Beginning
                Text{content: "QChart oldChart = chart();"}
            }
            InjectCode{
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (oldChart!=null && oldChart!=%1 && oldChart.parent()==null)\n"+
                              "    QtJambi_LibraryUtilities.internal.setJavaOwnership(%this);"}
            }
        }
    }
    
    ObjectType{
        name: "QtCharts::QDateTimeAxis"
    }
    
    ObjectType{
        name: "QtCharts::QBarModelMapper"
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
            signature: "setSeries(QtCharts::QAbstractBarSeries *)"
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
        ModifyFunction{
            signature: "model()const"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "series()const"
            access: Modification.NonFinal
        }
    }
    
    ObjectType{
        name: "QtCharts::QHBarModelMapper"
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
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
            signature: "setSeries(QtCharts::QAbstractBarSeries *)"
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
    }
    
    ObjectType{
        name: "QtCharts::QBoxPlotModelMapper"
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
            signature: "setSeries(QtCharts::QBoxPlotSeries *)"
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
        ModifyFunction{
            signature: "model()const"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "series()const"
            access: Modification.NonFinal
        }
    }
    
    ObjectType{
        name: "QtCharts::QHBoxPlotModelMapper"
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
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
            signature: "setSeries(QtCharts::QBoxPlotSeries *)"
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
    }
    
    ObjectType{
        name: "QtCharts::QHCandlestickModelMapper"
    }
    
    ObjectType{
        name: "QtCharts::QPieModelMapper"
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
            signature: "setSeries(QtCharts::QPieSeries *)"
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
        ModifyFunction{
            signature: "model()const"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "series()const"
            access: Modification.NonFinal
        }
    }
    
    ObjectType{
        name: "QtCharts::QHPieModelMapper"
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
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
            signature: "setSeries(QtCharts::QPieSeries *)"
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
    }
    
    ObjectType{
        name: "QtCharts::QXYModelMapper"
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
            signature: "setSeries(QtCharts::QXYSeries *)"
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
        ModifyFunction{
            signature: "model()const"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "series()const"
            access: Modification.NonFinal
        }
    }
    
    ObjectType{
        name: "QtCharts::QHXYModelMapper"
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
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
            signature: "setSeries(QtCharts::QXYSeries *)"
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
    }
    
    ObjectType{
        name: "QtCharts::QHorizontalBarSeries"
    }
    
    ObjectType{
        name: "QtCharts::QHorizontalPercentBarSeries"
    }
    
    ObjectType{
        name: "QtCharts::QHorizontalStackedBarSeries"
    }
    
    EnumType{
        name: "QtCharts::QLegend::MarkerShape"
    }
    
    ObjectType{
        name: "QtCharts::QLegend"
        ModifyFunction{
            signature: "setFont(QFont)"
            rename: "setLegendFont"
        }
        ModifyFunction{
            signature: "font()const"
            rename: "legendFont"
        }
    }
    
    EnumType{
        name: "QtCharts::QLegendMarker::LegendMarkerType"
    }
    
    ObjectType{
        name: "QtCharts::QLegendMarker"
    }
    
    ObjectType{
        name: "QtCharts::QLineSeries"
    }
    
    ObjectType{
        name: "QtCharts::QLogValueAxis"
    }
    
    ObjectType{
        name: "QtCharts::QPercentBarSeries"
    }
    
    ObjectType{
        name: "QtCharts::QPieLegendMarker"
    }
    
    ObjectType{
        name: "QtCharts::QPieSeries"
        ModifyFunction{
            signature: "append(QtCharts::QPieSlice *)"
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
            signature: "remove(QtCharts::QPieSlice *)"
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
            signature: "take(QtCharts::QPieSlice *)"
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
            signature: "insert(int,QtCharts::QPieSlice *)"
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
            signature: "append(QList<QtCharts::QPieSlice*>)"
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
            signature: "operator<<(QtCharts::QPieSlice*)"
            remove: RemoveFlag.All
        }
    }
    
    EnumType{
        name: "QtCharts::QPieSlice::LabelPosition"
    }
    
    ObjectType{
        name: "QtCharts::QPieSlice"
    }
    
    EnumType{
        name: "QtCharts::QPolarChart::PolarOrientation"
        flags: "QtCharts::QPolarChart::PolarOrientations"
    }
    
    ObjectType{
        name: "QtCharts::QPolarChart"
        ModifyFunction{
            signature: "axes(QFlags<QtCharts::QPolarChart::PolarOrientation>, QtCharts::QAbstractSeries *)const"
            ModifyArgument{
                index: 1
                RemoveDefaultExpression{
                }
            }
        }
        ModifyFunction{
            signature: "addAxis(QtCharts::QAbstractAxis *, QtCharts::QPolarChart::PolarOrientation)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiCharts.java"
                quoteAfterLine: "class QPolarChart___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    EnumType{
        name: "QtCharts::QScatterSeries::MarkerShape"
    }
    
    ObjectType{
        name: "QtCharts::QScatterSeries"
        ModifyFunction{
            signature: "brush()const"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QtCharts::QSplineSeries"
    }
    
    ObjectType{
        name: "QtCharts::QStackedBarSeries"
    }
    
    ObjectType{
        name: "QtCharts::QVBarModelMapper"
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
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
            signature: "setSeries(QtCharts::QAbstractBarSeries *)"
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
    }
    
    ObjectType{
        name: "QtCharts::QVBoxPlotModelMapper"
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
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
            signature: "setSeries(QtCharts::QBoxPlotSeries *)"
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
    }
    
    ObjectType{
        name: "QtCharts::QVCandlestickModelMapper"
    }
    
    ObjectType{
        name: "QtCharts::QVPieModelMapper"
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
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
            signature: "setSeries(QtCharts::QPieSeries *)"
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
    }
    
    ObjectType{
        name: "QtCharts::QVXYModelMapper"
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
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
            signature: "setSeries(QtCharts::QXYSeries *)"
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
    }
    
    EnumType{
        name: "QtCharts::QValueAxis::TickType"
    }
    
    ObjectType{
        name: "QtCharts::QValueAxis"
    }
    
    ObjectType{
        name: "QtCharts::QColorAxis"
    }
    
    ObjectType{
        name: "QtCharts::QXYLegendMarker"
    }
    
    EnumType{
        name: "QtCharts::QXYSeries::PointConfiguration"
        since: [6, 2]
    }
    
    ObjectType{
        name: "QtCharts::QXYSeries"
        ModifyFunction{
            signature: "markerSize() const"
            access: Modification.NonFinal
            since: [6, 2]
        }
        ModifyFunction{
            signature: "setMarkerSize(qreal)"
            access: Modification.NonFinal
            since: [6, 2]
        }
        ModifyFunction{
            signature: "replace(QVector<QPointF>)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "operator<<(QList<QPointF>)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator<<(QPointF)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "bestFitLineEquation(bool&)const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool %out = false;"}
                }
            }
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = __qt_%1 ? qtjambi_cast<jobject>(%env, %in) : nullptr;"}
                }
            }
            since: [6, 2]
        }
    }
}
