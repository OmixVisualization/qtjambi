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
    packageName: "io.qt.widgets"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtWidgets"
    module: "qtjambi"
    LoadTypeSystem{name: "QtGui"; unless: "QTJAMBI_NO_GUI"}
    Template{
        name: "gui.addAction"
        Text{content: "public final QAction addAction(%EXTRA String text %INFIX_EXTRA, Object receiver, String method %END_EXTRA %CONNECTION_TYPE) {\n"+
                      "    QAction returned = addAction(%CALL_EXTRA text %END_CALL_EXTRA);\n"+
                      "    %POST_CALL\n"+
                      "    returned.triggered.connect(receiver, method %USE_CONNECTION_TYPE);\n"+
                      "    return returned;\n"+
                      "}\n"+
                      "\n"+
                      "public final QAction addAction(%EXTRA String text %INFIX_EXTRA, io.qt.core.QMetaObject.Slot0 slot %END_EXTRA %CONNECTION_TYPE) {\n"+
                      "    QAction returned = addAction(%CALL_EXTRA text %END_CALL_EXTRA);\n"+
                      "    %POST_CALL\n"+
                      "    returned.triggered.connect(slot %USE_CONNECTION_TYPE);\n"+
                      "    return returned;\n"+
                      "}\n"+
                      "\n"+
                      "public final QAction addAction(%EXTRA String text %INFIX_EXTRA, io.qt.core.QMetaObject.Slot1<? super Boolean> slot %END_EXTRA %CONNECTION_TYPE) {\n"+
                      "    QAction returned = addAction(%CALL_EXTRA text %END_CALL_EXTRA);\n"+
                      "    %POST_CALL\n"+
                      "    returned.triggered.connect(slot %USE_CONNECTION_TYPE);\n"+
                      "    return returned;\n"+
                      "}\n"+
                      "\n"+
                      "public final QAction addAction(%EXTRA String text %INFIX_EXTRA, io.qt.core.QMetaObject.Connectable1<? super Boolean> signal %END_EXTRA %CONNECTION_TYPE) {\n"+
                      "    QAction returned = addAction(%CALL_EXTRA text %END_CALL_EXTRA);\n"+
                      "    %POST_CALL\n"+
                      "    returned.triggered.connect(signal %USE_CONNECTION_TYPE);\n"+
                      "    return returned;\n"+
                      "}"}
    }
    
    Template{
        name: "gui.addAction_with_shortcut"
        InsertTemplate{
            name: "gui.addAction"
            Replace{
                from: "%POST_CALL"
                to: "returned.setShortcut(shortcut);"
            }
            Replace{
                from: "%END_EXTRA"
                to: ", QKeySequence shortcut"
            }
            Replace{
                from: "%END_CALL_EXTRA"
                to: ""
            }
            Replace{
                from: "%INFIX_EXTRA"
                to: ""
            }
            Replace{
                from: "%CONNECTION_TYPE"
                to: ""
            }
            Replace{
                from: "%USE_CONNECTION_TYPE"
                to: ""
            }
        }
        InsertTemplate{
            name: "gui.addAction"
            Replace{
                from: "%POST_CALL"
                to: ""
            }
            Replace{
                from: "%END_EXTRA"
                to: ""
            }
            Replace{
                from: "%END_CALL_EXTRA"
                to: ""
            }
            Replace{
                from: "%INFIX_EXTRA"
                to: ""
            }
            Replace{
                from: "%CONNECTION_TYPE"
                to: ""
            }
            Replace{
                from: "%USE_CONNECTION_TYPE"
                to: ""
            }
        }
    }
    
    Template{
        name: "widget.addAction_with_shortcut"
        InsertTemplate{
            name: "gui.addAction"
            Replace{
                from: "%POST_CALL"
                to: "returned.setShortcut(shortcut);"
            }
            Replace{
                from: "%END_EXTRA"
                to: ""
            }
            Replace{
                from: "%END_CALL_EXTRA"
                to: ""
            }
            Replace{
                from: "%INFIX_EXTRA"
                to: ", QKeySequence shortcut"
            }
            Replace{
                from: "%CONNECTION_TYPE"
                to: ", Qt.ConnectionType... connectionType"
            }
            Replace{
                from: "%USE_CONNECTION_TYPE"
                to: ", connectionType"
            }
        }
        InsertTemplate{
            name: "gui.addAction"
            Replace{
                from: "%POST_CALL"
                to: ""
            }
            Replace{
                from: "%END_EXTRA"
                to: ""
            }
            Replace{
                from: "%END_CALL_EXTRA"
                to: ""
            }
            Replace{
                from: "%INFIX_EXTRA"
                to: ""
            }
            Replace{
                from: "%CONNECTION_TYPE"
                to: ", Qt.ConnectionType... connectionType"
            }
            Replace{
                from: "%USE_CONNECTION_TYPE"
                to: ", connectionType"
            }
        }
    }
    
    SuppressedWarning{text: "WARNING(JavaGenerator) :: Either add or remove specified for reference count variable '__rcItems' in 'QGraphicsGridLayout' but not both."}
    Rejection{
        className: "*"
        functionName: "d_func"
    }
    
    Rejection{
        className: "*"
        fieldName: "d_ptr"
    }
    
    Rejection{
        className: "*"
        fieldName: "d"
    }
    
    Rejection{
        className: ""
        functionName: "qgraphicsitem_cast"
    }
    
    Rejection{
        className: ""
        functionName: "qstyleoption_cast"
    }
    
    Rejection{
        className: "QFileDialogArgs"
    }
    
    Rejection{
        className: "QGraphicsItemPrivate"
    }
    
    Rejection{
        className: "QWidgetPrivate"
    }
    
    Rejection{
        className: "QMacCocoaViewContainer"
    }
    
    Rejection{
        className: "QMacNativeWidget"
    }
    
    Rejection{
        className: "QGtkStyle"
    }
    
    Rejection{
        className: "QWindowsCEStyle"
    }
    
    Rejection{
        className: "QWindowsMobileStyle"
    }
    
    Rejection{
        className: "QItemEditorCreator"
    }
    
    Rejection{
        className: "QMimeSource"
    }
    
    Rejection{
        className: "QUpdateLaterEvent"
    }
    
    Rejection{
        className: "QVFbHeader"
    }
    
    Rejection{
        className: "QWidgetData"
    }
    
    Rejection{
        className: "QWindowSurface"
    }
    
    Rejection{
        className: "QWindowsXPStyle"
    }
    
    Rejection{
        className: "QWindowsVistaStyle"
    }
    
    Rejection{
        className: "QWSEmbedWidget"
    }
    
    Rejection{
        className: "JObject_key"
    }
    
    Rejection{
        className: "QWidgetItemV2"
    }
    
    Rejection{
        className: "QActionGroup"
        functionName: "selected"
        until: 5
    }
    
    Rejection{
        className: "QGraphicsEffect"
        functionName: "source"
    }
    
    Rejection{
        className: "QPlatformFontDatabase"
        functionName: "fallbacksForFamily"
    }
    
    Rejection{
        className: "QPlatformWindowFormat"
        functionName: "setSharedContext"
    }
    
    Rejection{
        className: "QPlatformWindowFormat"
        functionName: "sharedGLContext"
    }
    
    Rejection{
        className: "QStandardItemEditorCreator"
    }
    
    Rejection{
        className: "QGraphicsItemGroup"
        enumName: "enum_1"
    }
    
    Rejection{
        className: "QGraphicsLineItem"
        enumName: "enum_1"
    }
    
    Rejection{
        className: "QGraphicsPathItem"
        enumName: "enum_1"
    }
    
    Rejection{
        className: "QGraphicsPixmapItem"
        enumName: "enum_1"
    }
    
    Rejection{
        className: "QGraphicsPolygonItem"
        enumName: "enum_1"
    }
    
    Rejection{
        className: "QGraphicsRectItem"
        enumName: "enum_1"
    }
    
    Rejection{
        className: "QGraphicsSimpleTextItem"
        enumName: "enum_1"
    }
    
    Rejection{
        className: "QGraphicsTextItem"
        enumName: "enum_1"
    }
    
    Rejection{
        className: "QGraphicsEllipseItem"
        enumName: "enum_1"
    }
    
    Rejection{
        className: "QGraphicsProxyWidget"
        enumName: "enum_1"
    }
    
    Rejection{
        className: "QGraphicsWidget"
        enumName: "enum_1"
    }
    
    Rejection{
        className: "QSizePolicy::Bits"
    }
    
    Rejection{
        className: "QAccessibleWidget"
        functionName: "interface_cast"
    }
    
    Rejection{
        className: "QAccessibleWidget"
        functionName: "tr"
    }
    
    Rejection{
        className: "QAccessibleWidget"
        functionName: "trUtf8"
    }
    
    Rejection{
        className: "QStyleHintReturn"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleHintReturn"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleHintReturnVariant"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleHintReturnVariant"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleHintReturnMask"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleHintReturnMask"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOption"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOption"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionComboBox"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionComboBox"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionComplex"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionComplex"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionDockWidget"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionDockWidget"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionFocusRect"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionFocusRect"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionFrame"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionFrame"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionGraphicsItem"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionGraphicsItem"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionGroupBox"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionGroupBox"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionHeader"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionHeader"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionHeaderV2"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionHeaderV2"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionMenuItem"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionMenuItem"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionProgressBar"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionProgressBar"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionRubberBand"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionRubberBand"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionSlider"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionSlider"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionSpinBox"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionSpinBox"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionTab"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionTab"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionTabV4"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionTabBarBase"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionTabBarBase"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionTabWidgetFrame"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionTabWidgetFrame"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionTitleBar"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionTitleBar"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionToolBar"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionToolBar"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionToolBox"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionToolBox"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionToolButton"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionToolButton"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOptionViewItem"
        enumName: "StyleOptionType"
    }
    
    Rejection{
        className: "QStyleOptionViewItem"
        enumName: "StyleOptionVersion"
    }
    
    Rejection{
        className: "QStyleOption"
        enumName: "OptionType"
    }
    
    Rejection{
        className: "QListWidgetItem"
        enumName: "ItemType"
    }
    
    Rejection{
        className: "QTableWidgetItem"
        enumName: "ItemType"
    }
    
    EnumType{
        name: "QInputDialog::InputMode"
    }
    
    EnumType{
        name: "QInputDialog::InputDialogOption"
    }
    
    EnumType{
        name: "QColorDialog::ColorDialogOption"
    }
    
    EnumType{
        name: "QAbstractItemDelegate::EndEditHint"
    }
    
    EnumType{
        name: "QAbstractItemView::CursorAction"
    }
    
    EnumType{
        name: "QAbstractItemView::DragDropMode"
    }
    
    EnumType{
        name: "QAbstractItemView::DropIndicatorPosition"
    }
    
    EnumType{
        name: "QAbstractItemView::EditTrigger"
    }
    
    EnumType{
        name: "QAbstractItemView::ScrollHint"
    }
    
    EnumType{
        name: "QAbstractItemView::ScrollMode"
    }
    
    EnumType{
        name: "QAbstractItemView::SelectionBehavior"
    }
    
    EnumType{
        name: "QAbstractItemView::SelectionMode"
    }
    
    EnumType{
        name: "QAbstractItemView::State"
    }
    
    EnumType{
        name: "QAbstractSlider::SliderAction"
    }
    
    EnumType{
        name: "QAbstractSlider::SliderChange"
    }
    
    EnumType{
        name: "QDirModel::Roles"
        until: 5
    }
    
    EnumType{
        name: "QGraphicsItem::CacheMode"
    }
    
    EnumType{
        name: "QMdiArea::AreaOption"
    }
    
    EnumType{
        name: "QMdiArea::WindowOrder"
    }
    
    EnumType{
        name: "QMdiArea::ViewMode"
    }
    
    EnumType{
        name: "QPlainTextEdit::LineWrapMode"
    }
    
    EnumType{
        name: "QMdiSubWindow::SubWindowOption"
    }
    
    EnumType{
        name: "QAction::ActionEvent"
        until: 5
    }
    
    EnumType{
        name: "QAction::MenuRole"
        until: 5
    }
    
    EnumType{
        name: "QAction::Priority"
        until: 5
    }
    
    EnumType{
        name: "QCalendarWidget::HorizontalHeaderFormat"
    }
    
    EnumType{
        name: "QCalendarWidget::SelectionMode"
    }
    
    EnumType{
        name: "QCalendarWidget::VerticalHeaderFormat"
    }
    
    EnumType{
        name: "QComboBox::InsertPolicy"
    }
    
    EnumType{
        name: "QComboBox::SizeAdjustPolicy"
    }
    
    EnumType{
        name: "QCompleter::CompletionMode"
    }
    
    EnumType{
        name: "QCompleter::ModelSorting"
    }
    
    EnumType{
        name: "QDataWidgetMapper::SubmitPolicy"
    }
    
    EnumType{
        name: "QDateTimeEdit::Section"
    }
    
    EnumType{
        name: "QDialog::DialogCode"
    }
    
    EnumType{
        name: "QDialogButtonBox::ButtonLayout"
    }
    
    EnumType{
        name: "QDialogButtonBox::ButtonRole"
    }
    
    EnumType{
        name: "QFileDialog::AcceptMode"
    }
    
    EnumType{
        name: "QFileDialog::DialogLabel"
    }
    
    EnumType{
        name: "QFileDialog::FileMode"
    }
    
    EnumType{
        name: "QFileDialog::Option"
    }
    
    EnumType{
        name: "QFileDialog::ViewMode"
    }
    
    EnumType{
        name: "QFileIconProvider::IconType"
    }
    
    EnumType{
        name: "QFileIconProvider::Option"
    }
    
    EnumType{
        name: "QFontComboBox::FontFilter"
    }
    
    EnumType{
        name: "QFrame::Shadow"
        extensible: true
    }
    
    EnumType{
        name: "QFrame::Shape"
    }
    
    EnumType{
        name: "QFrame::StyleMask"
    }
    
    EnumType{
        name: "QGraphicsItem::Extension"
        extensible: true
    }
    
    EnumType{
        name: "QGraphicsItem::GraphicsItemChange"
    }
    
    EnumType{
        name: "QGraphicsItem::GraphicsItemFlag"
    }
    
    EnumType{
        name: "QGraphicsPixmapItem::ShapeMode"
    }
    
    EnumType{
        name: "QGraphicsScene::ItemIndexMethod"
    }
    
    EnumType{
        name: "QGraphicsSceneContextMenuEvent::Reason"
    }
    
    EnumType{
        name: "QGraphicsView::CacheModeFlag"
    }
    
    EnumType{
        name: "QGraphicsView::DragMode"
    }
    
    EnumType{
        name: "QGraphicsView::OptimizationFlag"
    }
    
    EnumType{
        name: "QGraphicsView::ViewportAnchor"
    }
    
    EnumType{
        name: "QGraphicsView::ViewportUpdateMode"
    }
    
    EnumType{
        name: "QLCDNumber::Mode"
    }
    
    EnumType{
        name: "QLCDNumber::SegmentStyle"
    }
    
    EnumType{
        name: "QLayout::SizeConstraint"
    }
    
    EnumType{
        name: "QLineEdit::EchoMode"
    }
    
    EnumType{
        name: "QListView::Flow"
    }
    
    EnumType{
        name: "QListView::LayoutMode"
    }
    
    EnumType{
        name: "QListView::Movement"
    }
    
    EnumType{
        name: "QListView::ResizeMode"
    }
    
    EnumType{
        name: "QListView::ViewMode"
    }
    
    EnumType{
        name: "QMainWindow::DockOption"
    }
    
    EnumType{
        name: "QMessageBox::ButtonRole"
    }
    
    EnumType{
        name: "QMessageBox::Icon"
    }
    
    EnumType{
        name: "QProgressBar::Direction"
    }
    
    EnumType{
        name: "QRubberBand::Shape"
    }
    
    EnumType{
        name: "QSizePolicy::PolicyFlag"
    }
    
    EnumType{
        name: "QSizePolicy::Policy"
    }
    
    EnumType{
        name: "QSizePolicy::ControlType"
    }
    
    EnumType{
        name: "QStyle::SubControl"
        extensible: true
        RejectEnumValue{
            name: "SC_SpinBoxUp"
        }
        RejectEnumValue{
            name: "SC_SpinBoxDown"
        }
        RejectEnumValue{
            name: "SC_SpinBoxFrame"
        }
        RejectEnumValue{
            name: "SC_SpinBoxEditField"
        }
        RejectEnumValue{
            name: "SC_ComboBoxFrame"
        }
        RejectEnumValue{
            name: "SC_ComboBoxEditField"
        }
        RejectEnumValue{
            name: "SC_ComboBoxArrow"
        }
        RejectEnumValue{
            name: "SC_ComboBoxListBoxPopup"
        }
        RejectEnumValue{
            name: "SC_SliderGroove"
        }
        RejectEnumValue{
            name: "SC_SliderHandle"
        }
        RejectEnumValue{
            name: "SC_SliderTickmarks"
        }
        RejectEnumValue{
            name: "SC_ToolButton"
        }
        RejectEnumValue{
            name: "SC_ToolButtonMenu"
        }
        RejectEnumValue{
            name: "SC_TitleBarSysMenu"
        }
        RejectEnumValue{
            name: "SC_TitleBarMinButton"
        }
        RejectEnumValue{
            name: "SC_TitleBarMaxButton"
        }
        RejectEnumValue{
            name: "SC_TitleBarCloseButton"
        }
        RejectEnumValue{
            name: "SC_TitleBarNormalButton"
        }
        RejectEnumValue{
            name: "SC_TitleBarShadeButton"
        }
        RejectEnumValue{
            name: "SC_TitleBarUnshadeButton"
        }
        RejectEnumValue{
            name: "SC_TitleBarContextHelpButton"
        }
        RejectEnumValue{
            name: "SC_DialGroove"
        }
        RejectEnumValue{
            name: "SC_DialHandle"
        }
        RejectEnumValue{
            name: "SC_DialTickmarks"
        }
        RejectEnumValue{
            name: "SC_GroupBoxCheckBox"
        }
        RejectEnumValue{
            name: "SC_GroupBoxLabel"
        }
        RejectEnumValue{
            name: "SC_GroupBoxContents"
        }
        RejectEnumValue{
            name: "SC_GroupBoxFrame"
        }
        RejectEnumValue{
            name: "SC_MdiMinButton"
        }
        RejectEnumValue{
            name: "SC_MdiNormalButton"
        }
        RejectEnumValue{
            name: "SC_MdiCloseButton"
        }
    }
    
    EnumType{
        name: "QStyle::ComplexControl"
        extensible: true
    }
    
    EnumType{
        name: "QStyle::ContentsType"
        extensible: true
    }
    
    EnumType{
        name: "QStyle::ControlElement"
        extensible: true
    }
    
    EnumType{
        name: "QStyle::PixelMetric"
        extensible: true
        RejectEnumValue{
            name: "PM_MDIFrameWidth"
        }
        RejectEnumValue{
            name: "PM_MDIMinimizedWidth"
        }
    }
    
    EnumType{
        name: "QStyle::PrimitiveElement"
        extensible: true
        RejectEnumValue{
            name: "PE_IndicatorViewItemCheck"
        }
        RejectEnumValue{
            name: "PE_FrameStatusBar"
        }
        RejectEnumValue{
            name: "PE_IndicatorTabTearLeft"
            since: [5, 7]
        }
    }
    
    EnumType{
        name: "QStyle::StandardPixmap"
        extensible: true
    }
    
    EnumType{
        name: "QStyle::StateFlag"
    }
    
    EnumType{
        name: "QStyle::SubElement"
        extensible: true
        RejectEnumValue{
            name: "SE_ViewItemCheckIndicator"
        }
        RejectEnumValue{
            name: "SE_TabBarTearIndicatorLeft"
            since: [5, 7]
        }
    }
    
    EnumType{
        name: "QStyleHintReturn::HintReturnType"
    }
    
    EnumType{
        name: "QStyleOptionHeader::SectionPosition"
    }
    
    EnumType{
        name: "QStyleOptionHeader::SelectedPosition"
    }
    
    EnumType{
        name: "QStyleOptionHeader::SortIndicator"
    }
    
    EnumType{
        name: "QStyleOptionMenuItem::CheckType"
    }
    
    EnumType{
        name: "QStyleOptionMenuItem::MenuItemType"
    }
    
    EnumType{
        name: "QStyleOptionTab::CornerWidget"
    }
    
    EnumType{
        name: "QStyleOptionTab::SelectedPosition"
    }
    
    EnumType{
        name: "QStyleOptionTab::TabPosition"
    }
    
    EnumType{
        name: "QStyleOptionToolBar::ToolBarFeature"
    }
    
    EnumType{
        name: "QStyleOptionToolBar::ToolBarPosition"
    }
    
    EnumType{
        name: "QStyleOptionToolButton::ToolButtonFeature"
        RejectEnumValue{
            name: "MenuButtonPopup"
        }
    }
    
    EnumType{
        name: "QStyleOptionViewItem::Position"
    }
    
    EnumType{
        name: "QSystemTrayIcon::ActivationReason"
    }
    
    EnumType{
        name: "QSystemTrayIcon::MessageIcon"
    }
    
    EnumType{
        name: "QTabWidget::TabPosition"
    }
    
    EnumType{
        name: "QTabWidget::TabShape"
    }
    
    EnumType{
        name: "QToolButton::ToolButtonPopupMode"
    }
    
    EnumType{
        name: "QGraphicsScene::SceneLayer"
    }
    
    EnumType{
        name: "QWizard::WizardOption"
    }
    
    EnumType{
        name: "QWizard::WizardPixmap"
    }
    
    EnumType{
        name: "QWizard::WizardStyle"
    }
    
    EnumType{
        name: "QStackedLayout::StackingMode"
    }
    
    EnumType{
        name: "QGraphicsBlurEffect::BlurHint"
    }
    
    EnumType{
        name: "QGesture::GestureCancelPolicy"
    }
    
    EnumType{
        name: "QGraphicsEffect::PixmapPadMode"
    }
    
    EnumType{
        name: "QPinchGesture::ChangeFlag"
    }
    
    EnumType{
        name: "QGestureRecognizer::ResultFlag"
    }
    
    EnumType{
        name: "QWizard::WizardButton"
        RejectEnumValue{
            name: "NStandardButtons"
        }
        RejectEnumValue{
            name: "NButtons"
        }
    }
    
    EnumType{
        name: "QBoxLayout::Direction"
        RejectEnumValue{
            name: "Down"
        }
        RejectEnumValue{
            name: "Up"
        }
    }
    
    EnumType{
        name: "QDialogButtonBox::StandardButton"
        RejectEnumValue{
            name: "FirstButton"
        }
        RejectEnumValue{
            name: "LastButton"
        }
        RejectEnumValue{
            name: "YesAll"
        }
        RejectEnumValue{
            name: "NoAll"
        }
        RejectEnumValue{
            name: "Default"
        }
        RejectEnumValue{
            name: "Escape"
        }
        RejectEnumValue{
            name: "FlagMask"
        }
        RejectEnumValue{
            name: "ButtonMask"
        }
    }
    
    EnumType{
        name: "QDockWidget::DockWidgetFeature"
    }
    
    EnumType{
        name: "QMessageBox::StandardButton"
        RejectEnumValue{
            name: "FirstButton"
        }
        RejectEnumValue{
            name: "LastButton"
        }
        RejectEnumValue{
            name: "YesAll"
        }
        RejectEnumValue{
            name: "NoAll"
        }
    }
    
    EnumType{
        name: "QSlider::TickPosition"
        RejectEnumValue{
            name: "TicksLeft"
        }
        RejectEnumValue{
            name: "TicksRight"
        }
    }
    
    EnumType{
        name: "QStyle::StyleHint"
        extensible: true
        RejectEnumValue{
            name: "SH_ScrollBar_StopMouseOverSlider"
        }
    }
    
    EnumType{
        name: "QTextEdit::AutoFormattingFlag"
    }
    
    EnumType{
        name: "QTextEdit::LineWrapMode"
    }
    
    EnumType{
        name: "QGraphicsEffect::ChangeFlag"
    }
    
    EnumType{
        name: "QGraphicsItem::PanelModality"
    }
    
    EnumType{
        name: "QStyle::RequestSoftwareInputPanel"
    }
    
    EnumType{
        name: "QSwipeGesture::SwipeDirection"
    }
    
    EnumType{
        name: "QStyleOptionViewItem::ViewItemFeature"
    }
    
    EnumType{
        name: "QStyleOptionViewItem::ViewItemPosition"
    }
    
    EnumType{
        name: "QStyleOptionToolBox::SelectedPosition"
    }
    
    EnumType{
        name: "QStyleOptionToolBox::TabPosition"
    }
    
    EnumType{
        name: "QStyleOptionTab::TabFeature"
    }
    
    EnumType{
        name: "QColormap::Mode"
    }
    
    EnumType{
        name: "QStyleOptionFrame::FrameFeature"
    }
    
    EnumType{
        name: "QLineEdit::ActionPosition"
    }
    
    EnumType{
        name: "QAbstractScrollArea::SizeAdjustPolicy"
    }
    
    ValueType{
        name: "QTileRules"
    }
    
    ValueType{
        name: "QStyleOption"
        isPolymorphicBase: true
        polymorphicIdExpression: "%1->type == QStyleOption::SO_Default"
        ModifyField{
            name: "type"
            read: false
            write: false
        }
        ModifyField{
            name: "version"
            read: false
            write: false
        }
        ModifyField{
            name: "styleObject"
            read: true
            write: true
            ReferenceCount{
                action: ReferenceCount.Set
                variableName: "__rcStyleObject"
            }
        }
        ModifyFunction{
            signature: "QStyleOption(int, int)"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
            }
        }
        ModifyFunction{
            signature: "operator=(const QStyleOption &)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
    }
    
    ValueType{
        name: "QStyleOptionComplex"
        polymorphicIdExpression: "%1->type == QStyleOptionComplex::Type"
        ModifyFunction{
            signature: "QStyleOptionComplex(int, int)"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
            }
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionComplex &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionGraphicsItem"
        polymorphicIdExpression: "%1->type == QStyleOptionGraphicsItem::Type"
        ModifyFunction{
            signature: "QStyleOptionGraphicsItem(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionGraphicsItem &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionSizeGrip"
        Rejection{enumName: "StyleOptionType"}
        Rejection{enumName: "StyleOptionVersion"}
        polymorphicIdExpression: "%1->type == QStyleOptionSizeGrip::Type"
        ModifyFunction{
            signature: "QStyleOptionSizeGrip(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionSizeGrip &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionButton"
        Rejection{enumName: "StyleOptionType"}
        Rejection{enumName: "StyleOptionVersion"}
        EnumType{
            name: "ButtonFeature"
        }
        polymorphicIdExpression: "%1->type == QStyleOptionButton::Type"
        ModifyFunction{
            signature: "QStyleOptionButton(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionButton &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionComboBox"
        polymorphicIdExpression: "%1->type == QStyleOptionComboBox::Type"
        ModifyFunction{
            signature: "QStyleOptionComboBox(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionComboBox &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionDockWidget"
        polymorphicIdExpression: "%1->type == QStyleOptionDockWidget::Type"
        ModifyFunction{
            signature: "QStyleOptionDockWidget(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionDockWidget &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionFocusRect"
        polymorphicIdExpression: "%1->type == QStyleOptionFocusRect::Type"
        ModifyFunction{
            signature: "QStyleOptionFocusRect(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionFocusRect &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionFrame"
        polymorphicIdExpression: "%1->type == QStyleOptionFrame::Type"
        ModifyFunction{
            signature: "QStyleOptionFrame(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionFrame &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionGroupBox"
        polymorphicIdExpression: "%1->type == QStyleOptionGroupBox::Type"
        ModifyFunction{
            signature: "QStyleOptionGroupBox(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionGroupBox &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionHeader"
        polymorphicIdExpression: "%1->type == QStyleOptionHeader::Type"
        ModifyFunction{
            signature: "QStyleOptionHeader(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionHeader &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionHeaderV2"
        polymorphicIdExpression: "%1->type == QStyleOptionHeaderV2::Type && %1->version == QStyleOptionHeaderV2::Version"
        ModifyFunction{
            signature: "operator=(const QStyleOptionHeaderV2 &)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        ModifyFunction{
            signature: "QStyleOptionHeaderV2(int)"
            remove: RemoveFlag.All
        }
        ModifyField{
            name: "unused"
            read: false
            write: false
        }
        since: [6, 1]
    }
    
    ValueType{
        name: "QStyleOptionMenuItem"
        polymorphicIdExpression: "%1->type == QStyleOptionMenuItem::Type"
        ModifyFunction{
            signature: "QStyleOptionMenuItem(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionMenuItem &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionProgressBar"
        polymorphicIdExpression: "%1->type == QStyleOptionProgressBar::Type"
        ModifyFunction{
            signature: "QStyleOptionProgressBar(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionProgressBar &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionRubberBand"
        polymorphicIdExpression: "%1->type == QStyleOptionRubberBand::Type"
        ModifyFunction{
            signature: "QStyleOptionRubberBand(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionRubberBand &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionSlider"
        polymorphicIdExpression: "%1->type == QStyleOptionSlider::Type"
        ModifyFunction{
            signature: "QStyleOptionSlider(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionSlider &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionSpinBox"
        polymorphicIdExpression: "%1->type == QStyleOptionSpinBox::Type"
        ModifyFunction{
            signature: "QStyleOptionSpinBox(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionSpinBox &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionTabV4"
        polymorphicIdExpression: "false"
    }
    
    ValueType{
        name: "QStyleOptionTab"
        polymorphicIdExpression: "%1->type == QStyleOptionTab::Type"
        ModifyFunction{
            signature: "QStyleOptionTab(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionTab &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionTabBarBase"
        polymorphicIdExpression: "%1->type == QStyleOptionTabBarBase::Type"
        ModifyFunction{
            signature: "QStyleOptionTabBarBase(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionTabBarBase &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionTabWidgetFrame"
        polymorphicIdExpression: "%1->type == QStyleOptionTabWidgetFrame::Type"
        ModifyFunction{
            signature: "QStyleOptionTabWidgetFrame(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionTabWidgetFrame &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionTitleBar"
        polymorphicIdExpression: "%1->type == QStyleOptionTitleBar::Type"
        ModifyFunction{
            signature: "QStyleOptionTitleBar(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionTitleBar &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionToolBar"
        polymorphicIdExpression: "%1->type == QStyleOptionToolBar::Type"
        ModifyFunction{
            signature: "QStyleOptionToolBar(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionToolBar &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionToolBox"
        polymorphicIdExpression: "%1->type == QStyleOptionToolBox::Type"
        ModifyFunction{
            signature: "QStyleOptionToolBox(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionToolBox &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionToolButton"
        polymorphicIdExpression: "%1->type == QStyleOptionToolButton::Type"
        ModifyFunction{
            signature: "QStyleOptionToolButton(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionToolButton &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QStyleOptionViewItem"
        polymorphicIdExpression: "%1->type == QStyleOptionViewItem::Type"
        ModifyFunction{
            signature: "QStyleOptionViewItem(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QStyleOptionViewItem &)"
            Delegate{
                name: "set"
                deprecated: true
            }
            since: [5, 12]
        }
        ModifyField{
            name: "widget"
            ReferenceCount{
                action: ReferenceCount.Set
                variableName: "__rcWidget"
            }
        }
    }
    
    ValueType{
        name: "QSizePolicy"
    }
    
    ValueType{
        name: "QTableWidgetSelectionRange"
        CustomConstructor{
            type: CustomConstructor.Copy
            Text{content: "new(placement) QTableWidgetSelectionRange(copy->topRow(), copy->leftColumn(), copy->bottomRow(), copy->rightColumn());"}
        }
        CustomConstructor{
            type: CustomConstructor.Move
            Text{content: "new(placement) QTableWidgetSelectionRange(copy->topRow(), copy->leftColumn(), copy->bottomRow(), copy->rightColumn());"}
        }
    }
    
    ValueType{
        name: "QColormap"
        ModifyFunction{
            signature: "initialize()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "cleanup()"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QColor"
                location: Include.Global
            }
        }
        CustomConstructor{
            Text{content: "if(copy)\n"+
                          "    return new(placement) QColormap(*copy);\n"+
                          "else\n"+
                          "    return new(placement) QColormap(QColormap::instance());"}
        }
        CustomConstructor{
            type: CustomConstructor.Default
            Text{content: "new(placement) QColormap(QColormap::instance());"}
        }
    }
    
    InterfaceType{
        name: "QLayoutItem"
        ModifyField{
            name: "align"
            read: false
            write: false
        }
        ModifyFunction{
            signature: "spacerItem()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QSpacerItem"
    }
    
    ObjectType{
        name: "QWidgetItem"
        ModifyFunction{
            signature: "QWidgetItem(QWidget *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWidget"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyField{
            name: "wid"
            read: false
            write: true
            rename: "widget"
            ReferenceCount{
                action: ReferenceCount.Set
                variableName: "__rcWidget"
            }
        }
        InjectCode{
            position: Position.End
            Text{content: "@QtUninvokable\n"+
                          "static native boolean hasWidgetItemV2(QWidget widget);"}
        }
    }
    
    InterfaceType{
        name: "QGraphicsItem"
        Rejection{
            enumName: "enum_1"
        }
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "toGraphicsObject() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "type() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "paint(QPainter*,const QStyleOptionGraphicsItem*,QWidget*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "collidesWithItem(const QGraphicsItem*,Qt::ItemSelectionMode)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "contextMenuEvent(QGraphicsSceneContextMenuEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dragEnterEvent(QGraphicsSceneDragDropEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dragLeaveEvent(QGraphicsSceneDragDropEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dragMoveEvent(QGraphicsSceneDragDropEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dropEvent(QGraphicsSceneDragDropEvent*)"
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
        }
        ModifyFunction{
            signature: "focusOutEvent(QFocusEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "hoverEnterEvent(QGraphicsSceneHoverEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "hoverLeaveEvent(QGraphicsSceneHoverEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "hoverMoveEvent(QGraphicsSceneHoverEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "inputMethodEvent(QInputMethodEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "isObscuredBy(const QGraphicsItem*)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "keyPressEvent(QKeyEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "keyReleaseEvent(QKeyEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseMoveEvent(QGraphicsSceneMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mousePressEvent(QGraphicsSceneMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseReleaseEvent(QGraphicsSceneMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "sceneEvent(QEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "sceneEventFilter(QGraphicsItem*,QEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "wheelEvent(QGraphicsSceneWheelEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "installSceneEventFilter(QGraphicsItem *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeSceneEventFilter(QGraphicsItem *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "resetMatrix()"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setGraphicsEffect(QGraphicsEffect*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcGraphicsEffect"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setFocusProxy(QGraphicsItem*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        InjectCode{
            target: CodeClass.JavaInterface
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QGraphicsItem___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "QGraphicsItem(QGraphicsItem*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null)\n"+
                              "    QtJambi_LibraryUtilities.internal.setCppOwnership(%this);"}
            }
        }
        ModifyFunction{
            signature: "itemTransform(const QGraphicsItem*,bool*)const"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "// nothing"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool okHolder = false;\n"+
                                  "bool *%out = &okHolder;"}
                }
            }
            ModifyArgument{
                index: "return"
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "if (%2 != 0) *%2 = %in != 0;"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = !okHolder ? qtjambi_cast<jobject>(%env, %in) : nullptr;"}
                }
            }
        }
        ModifyFunction{
            signature: "setGroup(QGraphicsItemGroup*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                position: Position.Beginning
                Text{content: "io.qt.widgets.QGraphicsItemGroup oldGroup = %this.group();"}
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 == null && oldGroup != null && oldGroup.parentItem() == null)\n"+
                              "    QtJambi_LibraryUtilities.internal.setDefaultOwnership(%this);\n"+
                              "else\n"+
                              "    QtJambi_LibraryUtilities.internal.setCppOwnership(%this);"}
            }
        }
        ModifyFunction{
            signature: "setParentItem(QGraphicsItem*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 == null)\n"+
                              "    QtJambi_LibraryUtilities.internal.setDefaultOwnership(%this);\n"+
                              "else\n"+
                              "    QtJambi_LibraryUtilities.internal.setCppOwnership(%this);"}
            }
        }
        ModifyFunction{
            signature: "isBlockedByModalPanel ( QGraphicsItem **) const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QGraphicsItem * modalPanel = nullptr;\n"+
                                  "QGraphicsItem ** %out = &modalPanel;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.widgets.QGraphicsItem$BlockedByModalPanelInfo"
                }
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtWidgets::QGraphicsItem$BlockedByModalPanelInfo::newInstance(%env, jboolean(%in), qtjambi_cast<jobject>(%env, modalPanel));"}
                }
            }
        }
        ModifyFunction{
            signature: "commonAncestorItem(const QGraphicsItem*)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "focusItem()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "focusProxy()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "focusScopeItem()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "group()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "panel()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "parentItem()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "topLevelItem()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "focusProxy()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "focusScopeItem()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "group()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "panel()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "parentItem()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "topLevelItem()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QAbstractGraphicsShapeItem"
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "resetMatrix()"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "QAbstractGraphicsShapeItem(QGraphicsItem*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
    }
    
    ObjectType{
        name: "QAbstractItemView"
        ModifyFunction{
            signature: "update()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "horizontalStepsPerItem()const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setHorizontalStepsPerItem(int)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setVerticalStepsPerItem(int)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "verticalStepsPerItem()const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "edit(QModelIndex,QAbstractItemView::EditTrigger,QEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "openPersistentEditor(QModelIndex)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "closePersistentEditor(QModelIndex)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setIndexWidget(QModelIndex, QWidget *)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "selectionCommand(QModelIndex,const QEvent*)const"
            threadAffinity: true
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "setIndexWidget(const QModelIndex &,QWidget *)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setItemDelegate(QAbstractItemDelegate *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcItemDelegate"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcItemModel"
                    action: ReferenceCount.Set
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "// setModel() creates a new selection model for the object\n"+
                              "QtJambi_LibraryUtilities.internal.setReferenceCount(this, io.qt.widgets.QAbstractItemView.class, \"__rcItemSelectionModel\", false, false, null);"}
            }
        }
        ModifyFunction{
            signature: "setSelectionModel(QItemSelectionModel *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcItemSelectionModel"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setItemDelegateForColumn(int,QAbstractItemDelegate*)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcDelegatesForColumns"
                    keyArgument: 1
                    action: ReferenceCount.Put
                }
            }
        }
        ModifyFunction{
            signature: "setItemDelegateForRow(int,QAbstractItemDelegate*)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcDelegatesForRows"
                    keyArgument: 1
                    action: ReferenceCount.Put
                }
            }
        }
    }
    
    ObjectType{
        name: "QAbstractSlider"
    }
    
    ObjectType{
        name: "QActionGroup"
        ModifyFunction{
            signature: "addAction(QAction*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcActions"
                    action: ReferenceCount.Add
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "QtJambi_LibraryUtilities.internal.setReferenceCount(%1, QAction.class, \"__rcActionGroup\", false, false, this);"}
            }
        }
        ModifyFunction{
            signature: "removeAction(QAction*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcActions"
                    action: ReferenceCount.Take
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "QtJambi_LibraryUtilities.internal.setReferenceCount(%1, QAction.class, \"__rcActionGroup\", false, false, null);"}
            }
        }
        until: 5
    }
    
    EnumType{
        name: "QActionGroup::ExclusionPolicy"
        since: [5, 14]
        until: 5
    }
    
    ObjectType{
        name: "QCheckBox"
    }
    
    
    
    ObjectType{
        name: "QCommonStyle"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
    }
    
    ObjectType{
        name: "QDataWidgetMapper"
        ModifyFunction{
            signature: "addMapping(QWidget*,int)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMappings"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "addMapping(QWidget*,int,QByteArray)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMappings"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeMapping(QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMappings"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "setItemDelegate(QAbstractItemDelegate*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcItemDelegate"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setModel(QAbstractItemModel*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcModel"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QDateEdit"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
    }
    
    ObjectType{
        name: "QDialog"
        ModifyFunction{
            signature: "setExtension(QWidget*)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "open()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "exec()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "extension()const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "orientation()const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setOrientation(Qt::Orientation)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "showExtension(bool)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setVisible(bool)"
            allowAsSlot: true
            threadAffinity: true
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QDialogButtonBox"
        ModifyFunction{
            signature: "addButton(QString,QDialogButtonBox::ButtonRole)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "addButton(QDialogButtonBox::StandardButton)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "addButton(QAbstractButton*,QDialogButtonBox::ButtonRole)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeButton(QAbstractButton*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QDirModel"
        ModifyFunction{
            signature: "parent()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setIconProvider(QFileIconProvider*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcIconProvider"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "iconProvider()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QFileIconProvider"
    }
    
    ObjectType{
        name: "QWizard"
        ModifyFunction{
            signature: "setVisible(bool)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setWizardStyle(QWizard::WizardStyle)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setDefaultProperty(const char *, const char *, const char *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setSideWidget(QWidget*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addPage(QWizardPage*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "button(QWizard::WizardButton)const"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setButton(QWizard::WizardButton,QAbstractButton*)"
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setPage(int,QWizardPage*)"
            ModifyArgument{
                index: 2
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QWizardPage"
        ModifyFunction{
            signature: "registerField(const QString &, QWidget *, const char *, const char *)"
            threadAffinity: Affinity.UI
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: "if(changedSignal!=null && !changedSignal.isEmpty()) {\n"+
                              "    io.qt.core.QMetaMethod sig = widget.metaObject().method(changedSignal);\n"+
                              "    if(sig.methodType()==io.qt.core.QMetaMethod.MethodType.Signal) {\n"+
                              "        changedSignal = \"2\"+sig.cppMethodSignature().toString();\n"+
                              "    }\n"+
                              "}"}
            }
        }
    }
    
    ObjectType{
        name: "QFocusFrame"
        ModifyFunction{
            signature: "setWidget(QWidget*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWidget"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QFontComboBox"
    }
    
    ObjectType{
        name: "QFontDialog"
        EnumType{
            name: "FontDialogOption"
        }

        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QFontDialog__java_"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "open(QObject*,const char*)"
            threadAffinity: true
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: "if(!member.startsWith(\"1\") || !member.startsWith(\"2\")) member = '1'+member;"}
            }
        }
        ModifyFunction{
            signature: "getFont(bool*,QFont,QWidget*,QString,QFontDialog::FontDialogOptions)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? qtjambi_cast<jobject>(%env, %in) : nullptr;"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok(false);\n"+
                                  "bool* %out = &ok;"}
                }
            }
        }
        ModifyFunction{
            signature: "getFont(bool*,QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? qtjambi_cast<jobject>(%env, %in) : nullptr;"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok(false);\n"+
                                  "bool* %out = &ok;"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveDefaultExpression{}
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsEllipseItem"
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "type() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QGraphicsEllipseItem(QGraphicsItem*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QGraphicsEllipseItem(const QRectF &,QGraphicsItem*)"
            ModifyArgument{
                index: 2
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if (%2 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QGraphicsEllipseItem(qreal,qreal,qreal,qreal,QGraphicsItem*)"
            ModifyArgument{
                index: 5
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 5
                    metaName: "%5"
                }
                Text{content: "if (%5 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsItemAnimation"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "setItem(QGraphicsItem*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcItem"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setTimeLine(QTimeLine*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcTimeLine"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "item()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsItemGroup"
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "type() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "resetMatrix()"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setFocusProxy(QGraphicsItem*)"
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
            signature: "QGraphicsItemGroup(QGraphicsItem*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "addToGroup(QGraphicsItem*)"
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
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "boolean _hasParent = %1.parentItem()!=null;"}
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (!_hasParent && (%1.toGraphicsObject()==null || %1.toGraphicsObject().parent() == null) )\n"+
                              "    QtJambi_LibraryUtilities.internal.setCppOwnership(%1);"}
            }
        }
        ModifyFunction{
            signature: "removeFromGroup(QGraphicsItem*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1.parentItem() == null && (%1.toGraphicsObject()==null || %1.toGraphicsObject().parent() == null) )\n"+
                              "    QtJambi_LibraryUtilities.internal.setDefaultOwnership(%1);"}
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsLineItem"
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "type() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "resetMatrix()"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setFocusProxy(QGraphicsItem*)"
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
            signature: "QGraphicsLineItem(QGraphicsItem*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QGraphicsLineItem(const QLineF &,QGraphicsItem*)"
            ModifyArgument{
                index: 2
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if (%2 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QGraphicsLineItem(qreal,qreal,qreal,qreal,QGraphicsItem*)"
            ModifyArgument{
                index: 5
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 5
                    metaName: "%5"
                }
                Text{content: "if (%5 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsPathItem"
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "type() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QGraphicsPathItem(QGraphicsItem*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QGraphicsPathItem(const QPainterPath &,QGraphicsItem*)"
            ModifyArgument{
                index: 2
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if (%2 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsPixmapItem"
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "type() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "resetMatrix()"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setFocusProxy(QGraphicsItem*)"
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
            signature: "QGraphicsPixmapItem(QGraphicsItem*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QGraphicsPixmapItem(const QPixmap &,QGraphicsItem*)"
            ModifyArgument{
                index: 2
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if (%2 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsPolygonItem"
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "type() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QGraphicsPolygonItem(QGraphicsItem*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QGraphicsPolygonItem(const QPolygonF &,QGraphicsItem*)"
            ModifyArgument{
                index: 2
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if (%2 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsRectItem"
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "type() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QGraphicsRectItem(QGraphicsItem*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QGraphicsRectItem(qreal,qreal,qreal,qreal,QGraphicsItem*)"
            ModifyArgument{
                index: 5
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 5
                    metaName: "%5"
                }
                Text{content: "if (%5 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QGraphicsRectItem(const QRectF &,QGraphicsItem*)"
            ModifyArgument{
                index: 2
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if (%2 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsSimpleTextItem"
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "type() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QGraphicsSimpleTextItem(QGraphicsItem*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QGraphicsSimpleTextItem(const QString &,QGraphicsItem*)"
            ModifyArgument{
                index: 2
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if (%2 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
    }
    
    ObjectType{
        name: "QHBoxLayout"
    }
    
    ObjectType{
        name: "QHeaderView"
        EnumType{
            name: "ResizeMode"
            RejectEnumValue{
                name: "Custom"
            }
        }
        ModifyFunction{
            signature: "setVisible(bool)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "initStyleOptionForIndex(QStyleOptionHeader*,int)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            since: 6
        }
        ModifyFunction{
            signature: "paintSection(QPainter*,QRect,int)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "setModel(QAbstractItemModel*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcItemModel"
                    declareVariable: "QAbstractItemView"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QItemDelegate"
        ModifyFunction{
            signature: "createEditor(QWidget *,const QStyleOptionViewItem &, const QModelIndex &)const"
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
            signature: "itemEditorFactory()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "doLayout(QStyleOptionViewItem,QRect*,QRect*,QRect*,bool)const"
            Remove{
            }
        }
        ModifyFunction{
            signature: "drawCheck(QPainter*,QStyleOptionViewItem,QRect,Qt::CheckState)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "drawDecoration(QPainter*,QStyleOptionViewItem,QRect,QPixmap)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "drawDisplay(QPainter*,QStyleOptionViewItem,QRect,QString)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "drawFocus(QPainter*,QStyleOptionViewItem,QRect)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "selected(QPixmap,QPalette,bool)const"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "setItemEditorFactory(QItemEditorFactory*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcItemEditorFactory"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setEditorData(QWidget*,QModelIndex)const"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setModelData(QWidget*,QAbstractItemModel*,QModelIndex)const"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    InterfaceType{
        name: "QItemEditorCreatorBase"
        ModifyFunction{
            signature: "createWidget(QWidget *)const"
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
        name: "QItemEditorFactory"
        ModifyFunction{
            signature: "createEditor(int,QWidget *)const"
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
            signature: "registerEditor(int, QItemEditorCreatorBase *)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setDefaultFactory(QItemEditorFactory*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "defaultFactory()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QListView"
    }
    
    ObjectType{
        name: "QColumnView"
        ModifyFunction{
            signature: "createColumn(const QModelIndex&)"
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
            signature: "setPreviewWidget(QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setModel(QAbstractItemModel*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcItemModel"
                    declareVariable: "QAbstractItemView"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSelectionModel(QItemSelectionModel*)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcItemSelectionModel"
                    declareVariable: "QAbstractItemView"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QMainWindow"
        ModifyFunction{
            signature: "addDockWidget(Qt::DockWidgetArea,QDockWidget*,Qt::Orientation)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addDockWidget(Qt::DockWidgetArea,QDockWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addToolBar(QToolBar*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addToolBar(Qt::ToolBarArea,QToolBar*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertToolBar(QToolBar*,QToolBar*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertToolBarBreak(QToolBar*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeDockWidget(QDockWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeToolBar(QToolBar*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeToolBarBreak(QToolBar*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setCentralWidget(QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "takeCentralWidget()"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "setMenuBar(QMenuBar*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setMenuWidget(QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setStatusBar(QStatusBar*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "createPopupMenu()"
            threadAffinity: Affinity.UI
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
            signature: "addToolBar(QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "addToolBarBreak(Qt::ToolBarArea)"
            threadAffinity: Affinity.UI
        }
    }
    
    ObjectType{
        name: "QMdiArea"
        ModifyFunction{
            signature: "setViewMode(QMdiArea::ViewMode)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setDocumentMode(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setTabsClosable(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setTabsMovable(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setTabShape(QTabWidget::TabShape)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setTabPosition(QTabWidget::TabPosition)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "addSubWindow(QWidget*,Qt::WindowFlags)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeSubWindow(QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setActiveSubWindow(QMdiSubWindow*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setupViewport(QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QMdiSubWindow"
        ModifyFunction{
            signature: "setSystemMenu(QMenu*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setWidget(QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QMenu"
        ModifyFunction{
            signature: "initStyleOption(QStyleOptionMenuItem*,const QAction*)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            since: 6
        }
        ModifyFunction{
            signature: "addAction<Func1>(QIcon,QString,Func1,QKeySequence)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "addAction<Func1>(QString,Func1,QKeySequence)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setAsDockMenu()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "addAction(QAction *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "hideTearOffMenu()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "showTearOffMenu()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "showTearOffMenu(QPoint)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setSeparatorsCollapsible(bool)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setToolTipsVisible(bool)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "popup(QPoint, QAction *)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "exec()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "exec(QPoint, QAction *)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "exec(QList<QAction *>, QPoint, QAction *, QWidget *)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "clear()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setTearOffEnabled(bool)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "addMenu(QMenu *)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "addMenu(QString)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "addMenu(QIcon,QString)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "addSection(QString)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "addSection(QIcon,QString)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "addSeparator()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "addAction(QString)"
            threadAffinity: true
            until: [6, 2]
        }
        ModifyFunction{
            signature: "addAction(QIcon,QString)"
            threadAffinity: true
            until: [6, 2]
        }
        ModifyFunction{
            signature: "addMenu(QMenu*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcMenus"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "insertMenu(QAction*,QMenu*)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcMenus"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "insertSection(QAction*,QIcon,QString)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertSection(QAction*,QString)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertSeparator(QAction*)"
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
            signature: "setActiveAction(QAction*)"
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
            signature: "setDefaultAction(QAction*)"
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
            signature: "setNoReplayFor(QWidget*)"
            threadAffinity: true
            Remove{
            }
        }
        ModifyFunction{
            signature: "addAction(QString,const QObject*,const char*,QKeySequence)"
            Remove{
            }
        }
        ModifyFunction{
            signature: "addAction(QIcon,QString,const QObject*,const char*,QKeySequence)"
            Remove{
            }
        }
        ExtraIncludes{
            Include{
                fileName: "io.qt.gui.*"
                location: Include.Java
            }
            until: [6, 2]
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QMenu___"
                quoteBeforeLine: "}// class"
            }
        }
        InjectCode{
            target: CodeClass.Native
            Text{content: "extern \"C\" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_widgets_QMenu_setAsOSXDockMenu)\n"+
                          " (JNIEnv *__jni_env, jobject __this)\n"+
                          "{\n"+
                          "#ifdef Q_OS_OSX\n"+
                          "    QMenu* __qt_this = qtjambi_cast<QMenu*>(__jni_env, __this);\n"+
                          "    QtJambiAPI::checkNullPointer(__jni_env, __qt_this);\n"+
                          "    __qt_this->setAsDockMenu();\n"+
                          "#else\n"+
                          "    Q_UNUSED(__jni_env)\n"+
                          "    Q_UNUSED(__this)\n"+
                          "#endif\n"+
                          "}"}
        }
        InjectCode{
            until: [6, 2]
            InsertTemplate{
                name: "gui.addAction_with_shortcut"
                Replace{
                    from: "%EXTRA"
                    to: ""
                }
                Replace{
                    from: "%CALL_EXTRA"
                    to: ""
                }
            }
            InsertTemplate{
                name: "gui.addAction_with_shortcut"
                Replace{
                    from: "%EXTRA"
                    to: "QIcon icon,"
                }
                Replace{
                    from: "%CALL_EXTRA"
                    to: "icon,"
                }
            }
        }
        ModifyFunction{
            signature: "clear()"
            InjectCode{
                position: Position.End
                Text{content: "if(__rcMenus!=null)__rcMenus.clear();\n"+
                              "    QtJambi_LibraryUtilities.internal.clearReferenceCount(this, QWidget.class, \"__rcActions\", false);"}
            }
        }
    }
    
    ObjectType{
        name: "QMenuBar"
        ModifyFunction{
            signature: "initStyleOption(QStyleOptionMenuItem*,const QAction*)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            since: 6
        }
        ModifyFunction{
            signature: "addAction<Func1>(QString,Func1)"
            remove: RemoveFlag.All
            until: 6.3
        }
        ModifyFunction{
            signature: "clear()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setNativeMenuBar(bool)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setVisible(bool)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "addAction(QString)"
            threadAffinity: true
            until: [6, 2]
        }
        ModifyFunction{
            signature: "addMenu(QString)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "addMenu(QIcon,QString)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "addSeparator()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "addAction(QAction *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "addAction(QString,const QObject*,const char*)"
            remove: RemoveFlag.All
            until: [6, 2]
        }
        ModifyFunction{
            signature: "addMenu(QMenu*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcMenus"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "insertMenu(QAction*,QMenu*)"
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
                    variableName: "__rcMenus"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "insertSeparator(QAction*)"
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
            signature: "setActiveAction(QAction*)"
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
            signature: "setCornerWidget(QWidget*,Qt::Corner)"
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
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ExtraIncludes{
            Include{
                fileName: "io.qt.gui.*"
                location: Include.Java
            }
            since: 6
            until: [6, 2]
        }
        InjectCode{
            until: [6, 2]
            InsertTemplate{
                name: "gui.addAction"
                Replace{
                    from: "%POST_CALL"
                    to: ""
                }
                Replace{
                    from: "%END_EXTRA"
                    to: ""
                }
                Replace{
                    from: "%END_CALL_EXTRA"
                    to: ""
                }
                Replace{
                    from: "%INFIX_EXTRA"
                    to: ""
                }
                Replace{
                    from: "%EXTRA"
                    to: ""
                }
                Replace{
                    from: "%CALL_EXTRA"
                    to: ""
                }
                Replace{
                    from: "%CONNECTION_TYPE"
                    to: ""
                }
                Replace{
                    from: "%USE_CONNECTION_TYPE"
                    to: ""
                }
            }
        }
        ModifyFunction{
            signature: "clear()"
            InjectCode{
                position: Position.End
                Text{content: "if(__rcMenus!=null)__rcMenus.clear();\n"+
                              "    QtJambi_LibraryUtilities.internal.clearReferenceCount(this, QWidget.class, \"__rcActions\", false);"}
            }
        }
    }
    
    
    
    
    
    ObjectType{
        name: "QProgressBar"
        ModifyFunction{
            signature: "textDirection()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "invertedAppearance()const"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QPushButton"
        ModifyFunction{
            signature: "setMenu(QMenu*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMenu"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QScrollArea"
        ModifyFunction{
            signature: "setWidget(QWidget*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "takeWidget()"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QShortcut"
        ModifyFunction{
            signature: "QShortcut(QKeySequence,QWidget*,const char*,const char*,Qt::ShortcutContext)"
            access: Modification.Private
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
            }
            ModifyArgument{
                index: 3
                RemoveDefaultExpression{
                }
                ReplaceType{
                    modifiedType: "java.lang.Object"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const char* %out = nullptr;\n"+
                                  "Q_UNUSED(%in)"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveDefaultExpression{
                }
                ReplaceType{
                    modifiedType: "java.lang.Object"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const char* %out = nullptr;\n"+
                                  "Q_UNUSED(%in)"}
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QShortcut__"
                quoteBeforeLine: "}// class"
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QSizeGrip"
        ModifyFunction{
            signature: "setVisible(bool)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    
    ObjectType{
        name: "QStatusBar"
        ModifyFunction{
            signature: "addPermanentWidget(QWidget *, int)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addWidget(QWidget *, int)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeWidget(QWidget *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertPermanentWidget(int, QWidget *, int)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertWidget(int, QWidget *, int)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QStyleFactory"
        ModifyFunction{
            signature: "create(QString)"
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
        name: "QStyleHintReturn"
        noImplicitConstructors: true
        ModifyFunction{
            signature: "QStyleHintReturn(int, int)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "1"
                }
            }
        }
    }
    
    ObjectType{
        name: "QStyleHintReturnVariant"
    }
    
    ObjectType{
        name: "QStyleHintReturnMask"
    }
    
    ObjectType{
        name: "QStylePainter"
    }
    
    ObjectType{
        name: "QSystemTrayIcon"
        ModifyFunction{
            signature: "setContextMenu(QMenu*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcContextMenu"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QTableView"
        ModifyFunction{
            signature: "setHorizontalHeader(QHeaderView*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setVerticalHeader(QHeaderView*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setModel(QAbstractItemModel*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcItemModel"
                    declareVariable: "QAbstractItemView"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSelectionModel(QItemSelectionModel*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcItemSelectionModel"
                    declareVariable: "QAbstractItemView"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "sortByColumn(int)"
            remove: RemoveFlag.All
            until: 5
        }
    }
    
    ValueType{
        name: "QTextEdit::ExtraSelection"
        Include{
            fileName: "QTextEdit"
            location: Include.Global
        }
    }
    
    ObjectType{
        name: "QTimeEdit"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
    }
    
    ObjectType{
        name: "QToolBox"
        ModifyFunction{
            signature: "addItem(QWidget*,QString)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addItem(QWidget*,QIcon,QString)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertItem(int,QWidget*,QIcon,QString)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertItem(int,QWidget*,QString)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setCurrentWidget(QWidget*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QToolButton"
        ModifyFunction{
            signature: "setDefaultAction(QAction *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcDefaultAction"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setMenu(QMenu *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMenu"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QToolTip"
    }
    
    ObjectType{
        name: "QTreeView"
        ModifyFunction{
            signature: "drawBranches(QPainter*,QRect,QModelIndex)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "drawRow(QPainter*,QStyleOptionViewItem,QModelIndex)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "setHeader(QHeaderView*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setModel(QAbstractItemModel*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcItemModel"
                    declareVariable: "QAbstractItemView"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSelectionModel(QItemSelectionModel*)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcItemSelectionModel"
                    declareVariable: "QAbstractItemView"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "sortByColumn(int)"
            remove: RemoveFlag.All
            until: 5
        }
    }
    
    ObjectType{
        name: "QUndoCommand"
        ModifyFunction{
            signature: "mergeWith(const QUndoCommand*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QUndoGroup"
        ModifyFunction{
            signature: "createUndoAction(QObject *, const QString &) const"
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
            signature: "createRedoAction(QObject *, const QString &) const"
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
            signature: "addStack(QUndoStack*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcStacks"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeStack(QUndoStack*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcStacks"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "setActiveStack(QUndoStack*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QUndoStack"
        ModifyFunction{
            signature: "createUndoAction(QObject *, const QString &) const"
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
            signature: "createRedoAction(QObject *, const QString &) const"
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
            signature: "push(QUndoCommand *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QUndoView"
        ModifyFunction{
            signature: "setGroup(QUndoGroup *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcGroupOrStack"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setStack(QUndoStack *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcGroupOrStack"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "QUndoView(QUndoGroup *,QWidget *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcGroupOrStack"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "QUndoView(QUndoStack *,QWidget *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcGroupOrStack"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QVBoxLayout"
    }
    
    ObjectType{
        name: "QWhatsThis"
        ModifyFunction{
            signature: "showText(QPoint, QString, QWidget *)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "createAction(QObject *)"
            ModifyArgument{
                index: 1
                threadAffinity: true
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
    
    ObjectType{
        name: "QWidgetAction"
        ModifyFunction{
            signature: "createWidget(QWidget*)"
            threadAffinity: Affinity.UI
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
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "setDefaultWidget(QWidget*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "QGestureEvent"
        ModifyFunction{
            signature: "accept()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "ignore()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "isAccepted()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setAccepted(bool)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setWidget(QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWidget"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setAccepted(QGesture*,bool)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcGesture"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsSceneContextMenuEvent"
    }
    
    ObjectType{
        name: "QGraphicsSceneDragDropEvent"
        ModifyFunction{
            signature: "setMimeData(const QMimeData *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMimeData"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSource(QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSource"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsSceneEvent"
        ModifyFunction{
            signature: "setWidget(QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWidget"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsSceneMoveEvent"
    }
    
    ObjectType{
        name: "QGraphicsSceneResizeEvent"
    }
    
    ObjectType{
        name: "QGraphicsSceneHelpEvent"
    }
    
    ObjectType{
        name: "QGraphicsSceneHoverEvent"
    }
    
    ObjectType{
        name: "QGraphicsSceneMouseEvent"
    }
    
    ObjectType{
        name: "QGraphicsSceneWheelEvent"
    }
    
    ObjectType{
        name: "QToolBarChangeEvent"
        ModifyFunction{
            signature: "operator=(const QToolBarChangeEvent &)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        ModifyFunction{
            signature: "clone()const"
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
            since: 6
        }
    }
    
    ObjectType{
        name: "QAbstractButton"
    }
    
    ObjectType{
        name: "QStyle"
        ModifyFunction{
            signature: "drawItemPixmap(QPainter*,QRect,int,QPixmap)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "drawItemText(QPainter*,QRect,int,QPalette,bool,QString,QPalette::ColorRole)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QStyle___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "drawControl(QStyle::ControlElement, const QStyleOption *, QPainter *, const QWidget *) const"
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "drawComplexControl(QStyle::ComplexControl, const QStyleOptionComplex *, QPainter *, const QWidget *) const"
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "drawPrimitive(QStyle::PrimitiveElement, const QStyleOption *, QPainter *, const QWidget *) const"
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "hitTestComplexControl(QStyle::ComplexControl, const QStyleOptionComplex *, const QPoint &, const QWidget *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.widgets.QStyle$SubControl"
                }
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "QStyle::SubControl %out = qtjambi_cast<QStyle::SubControl>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = Java::QtWidgets::QStyle::findSubControl(%env, %1, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "styleHint(QStyle::StyleHint, const QStyleOption *, const QWidget *, QStyleHintReturn *) const"
            ModifyArgument{
                index: 4
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "polish(QPalette&)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QColorDialog"
        ModifyFunction{
            signature: "setVisible(bool)"
            threadAffinity: true
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QColorDialog__java_"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "getColor(const QColor &, QWidget *, const QString &, QColorDialog::ColorDialogOptions)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "new io.qt.gui.QColor(io.qt.core.Qt.GlobalColor.white)"
                }
            }
        }
        ModifyFunction{
            signature: "open(QObject*,const char*)"
            threadAffinity: true
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: "if(!member.startsWith(\"1\") || !member.startsWith(\"2\")) member = '1'+member;"}
            }
        }
        ModifyFunction{
            signature: "getRgba(uint,bool*,QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.@Nullable Integer"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? QtJambiAPI::toJavaIntegerObject(%env, %in) : nullptr;"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok(false);\n"+
                                  "bool* %out = &ok;"}
                }
            }
            until: 5
        }
    }
    
    ObjectType{
        name: "QLayout"
        ModifyFunction{
            signature: "setSpacing(int)"
            rename: "setWidgetSpacing"
        }
        ModifyFunction{
            signature: "spacing()const"
            rename: "widgetSpacing"
        }
        ModifyFunction{
            signature: "addChildWidget(QWidget *)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    condition: "%1.parent()==null"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "itemAt(int)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Invalidate
                }
            }
        }
        ModifyFunction{
            signature: "replaceWidget(QWidget*,QWidget*,Qt::FindChildOptions)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Invalidate
                }
            }
        }
        ModifyFunction{
            signature: "indexOf(const QLayoutItem *) const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            since: [6, 1]
        }
        ModifyFunction{
            signature: "removeWidget(QWidget *)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "setAlignment(QWidget*,Qt::Alignment)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setAlignment(QLayout*,Qt::Alignment)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setMenuBar(QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMenuBar"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "margin()const"
            remove: RemoveFlag.All
            until: 5
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QLayout___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "takeAt(int)"
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
                position: Position.End
                ArgumentMap{
                    index: 0
                    metaName: "%item"
                }
                Text{content: "if(%item != null){\n"+
                              "    QtJambi_LibraryUtilities.internal.setDefaultOwnership(%item);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "addWidget(QWidget *)"
            rename: "add_widget"
            access: Modification.Friendly | Modification.NonFinal
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    condition: "%1.parent()==null"
                    action: ReferenceCount.Add
                }
            }
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%1))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
        }
        ModifyFunction{
            signature: "addItem(QLayoutItem *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Position1
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "QWidget widget = %1.widget();"}
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "if(widget!=null && widget.parent()==null){\n"+
                              "    QtJambi_LibraryUtilities.internal.addReferenceCount(this, io.qt.widgets.QLayout.class, \"__rcLayoutedObjects\", false, false, widget);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "removeItem(QLayoutItem *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Position1
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "QWidget widget = %1.widget();"}
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "if(widget!=null && __rcLayoutedObjects!=null){\n"+
                              "    __rcLayoutedObjects.remove(widget);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "addChildLayout(QLayout *)"
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
            signature: "addChildLayout(QLayout *)"
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
            signature: "getContentsMargins(int*,int*,int*,int*)const"
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(%1.length<4) throw new IllegalArgumentException(\"An array of length 4 expected.\");"}
            }
            ModifyArgument{
                index: 1
                rename: "margins"
                ReplaceType{
                    modifiedType: "int[]"
                }
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 4;\n"+
                                  "int* %out = qtjambi_array_cast<int*>(%env, %scope, %in, %out_size);"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int* %out = __qt_%1+1;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int* %out = __qt_%1+2;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int* %out = __qt_%1+3;"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QStackedLayout"
        ModifyFunction{
            signature: "itemAt(int) const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Invalidate
                }
            }
        }
        ModifyFunction{
            signature: "setCurrentWidget(QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "takeAt(int)"
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
                position: Position.End
                ArgumentMap{
                    index: 0
                    metaName: "%item"
                }
                Text{content: "if(%item != null){\n"+
                              "    QtJambi_LibraryUtilities.internal.setDefaultOwnership(%item);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "addWidget(QWidget *)"
            rename: "stackWidget"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%1.parent()==null"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "insertWidget(int,QWidget*)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%2.parent()==null"
                    action: ReferenceCount.Add
                }
            }
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%2))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
        }
        ModifyFunction{
            signature: "addItem(QLayoutItem *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Invalidate
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Position1
                ArgumentMap{
                    index: 1
                    metaName: "%item"
                }
                Text{content: "if(widget==null){\n"+
                              "    throw new IllegalArgumentException(\"Only widgets can be added to QStackedLayout. Use QWidgetItem instead.\");\n"+
                              "}"}
            }
        }
        InjectCode{
            position: Position.End
            Text{content: "@QtUninvokable\n"+
                          "void add_widget(QWidget widget){\n"+
                          "    stackWidget(widget);\n"+
                          "}"}
        }
    }
    
    ObjectType{
        name: "QBoxLayout"
        ModifyFunction{
            signature: "addItem(QLayoutItem *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertLayout(int, QLayout *, int)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addSpacerItem(QSpacerItem*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertSpacerItem(int,QSpacerItem*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "addLayout(QLayout *, int)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setStretchFactor(QWidget*,int)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setStretchFactor(QLayout*,int)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "itemAt(int) const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Invalidate
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QBoxLayout___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "takeAt(int)"
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
                position: Position.End
                ArgumentMap{
                    index: 0
                    metaName: "%item"
                }
                Text{content: "if(%item != null){\n"+
                              "QtJambi_LibraryUtilities.internal.setDefaultOwnership(%item);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "insertItem(int, QLayoutItem *)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Position1
                ArgumentMap{
                    index: 2
                    metaName: "%1"
                }
                Text{content: "QWidget widget = %1.widget();"}
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "if(widget!=null && widget.parent()==null){\n"+
                              "    QtJambi_LibraryUtilities.internal.addReferenceCount(this, io.qt.widgets.QLayout.class, \"__rcLayoutedObjects\", false, false, widget);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "addWidget(QWidget *, int, Qt::Alignment)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%1.parent()==null"
                    action: ReferenceCount.Add
                }
            }
            ModifyArgument{
                index: 2
                RemoveDefaultExpression{
                }
            }
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%1))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
        }
        ModifyFunction{
            signature: "insertWidget(int,QWidget*,int,Qt::Alignment)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%2.parent()==null"
                    action: ReferenceCount.Add
                }
            }
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%2))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
        }
    }
    
    ObjectType{
        name: "QGridLayout"
        ModifyFunction{
            signature: "addLayout(QLayout *, int, int, Qt::Alignment)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addLayout(QLayout *, int, int, int, int, Qt::Alignment)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "itemAt(int) const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Invalidate
                }
            }
        }
        ModifyFunction{
            signature: "itemAtPosition(int,int)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "takeAt(int)"
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
                position: Position.End
                ArgumentMap{
                    index: 0
                    metaName: "%item"
                }
                Text{content: "if(%item != null){\n"+
                              "    QtJambi_LibraryUtilities.internal.setDefaultOwnership(%item);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "addWidget(QWidget*,int,int,Qt::Alignment)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%1.parent()==null"
                    action: ReferenceCount.Add
                }
            }
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%1))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
        }
        ModifyFunction{
            signature: "addWidget(QWidget*,int,int,int,int,Qt::Alignment)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%1.parent()==null"
                    action: ReferenceCount.Add
                }
            }
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%1))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
        }
        ModifyFunction{
            signature: "addWidget(QWidget *)"
            remove: RemoveFlag.All
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%1.parent()==null"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "addItem(QLayoutItem *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "addItem(QLayoutItem *, int, int, int, int, Qt::Alignment)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Position1
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "QWidget widget = %1.widget();"}
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "if(widget!=null && widget.parent()==null){\n"+
                              "    QtJambi_LibraryUtilities.internal.addReferenceCount(this, io.qt.widgets.QLayout.class, \"__rcLayoutedObjects\", false, false, widget);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "getItemPosition(int,int*,int*,int*,int*)const"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint %in;\n"+
                                  "int* %out = reinterpret_cast<int*>(&%in);"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint %in;\n"+
                                  "int* %out = reinterpret_cast<int*>(&%in);"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint %in;\n"+
                                  "int* %out = reinterpret_cast<int*>(&%in);"}
                }
            }
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint %in;\n"+
                                  "int* %out = reinterpret_cast<int*>(&%in);"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.widgets.QGridLayout$ItemInfo"
                }
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtWidgets::QGridLayout$ItemInfo::newInstance(%env, %2, %3, %4, %5);"}
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QGridLayout___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QFormLayout"
        ValueType{
            name: "TakeRowResult"
            ModifyField{
                name: "labelItem"
                read: true
                write: false
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
            ModifyField{
                name: "fieldItem"
                read: true
                write: false
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        EnumType{
            name: "FieldGrowthPolicy"
        }

        EnumType{
            name: "ItemRole"
        }

        EnumType{
            name: "RowWrapPolicy"
        }
        ModifyFunction{
            signature: "itemAt(int) const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Invalidate
                }
            }
        }
        ModifyFunction{
            signature: "itemAt(int,QFormLayout::ItemRole) const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "takeAt(int)"
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
                position: Position.End
                ArgumentMap{
                    index: 0
                    metaName: "%item"
                }
                Text{content: "if(%item != null){\n"+
                              "QtJambi_LibraryUtilities.internal.setDefaultOwnership(%item);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "addItem(QLayoutItem *)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "getLayoutPosition(QLayout*,int*,QFormLayout::ItemRole*)const"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint %in;\n"+
                                  "int* %out = reinterpret_cast<int*>(&%in);"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint %in;\n"+
                                  "QFormLayout::ItemRole* %out = reinterpret_cast<QFormLayout::ItemRole*>(&%in);"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.widgets.QFormLayout$ItemInfo"
                }
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtWidgets::QFormLayout$ItemInfo::newInstance(%env, %2, qtjambi_cast<jobject>(%env, %3));"}
                }
            }
        }
        ModifyFunction{
            signature: "getWidgetPosition(QWidget*,int*,QFormLayout::ItemRole*)const"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint %in;\n"+
                                  "int* %out = reinterpret_cast<int*>(&%in);"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint %in;\n"+
                                  "QFormLayout::ItemRole* %out = reinterpret_cast<QFormLayout::ItemRole*>(&%in);"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.widgets.QFormLayout$ItemInfo"
                }
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtWidgets::QFormLayout$ItemInfo::newInstance(%env, %2, qtjambi_cast<jobject>(%env, %3));"}
                }
            }
        }
        ModifyFunction{
            signature: "getItemPosition(int,int*,QFormLayout::ItemRole*)const"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint %in;\n"+
                                  "int* %out = reinterpret_cast<int*>(&%in);"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint %in;\n"+
                                  "QFormLayout::ItemRole* %out = reinterpret_cast<QFormLayout::ItemRole*>(&%in);"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.widgets.QFormLayout$ItemInfo"
                }
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtWidgets::QFormLayout$ItemInfo::newInstance(%env, %2, qtjambi_cast<jobject>(%env, %3));"}
                }
            }
        }
        ModifyFunction{
            signature: "setItem(int,QFormLayout::ItemRole,QLayoutItem*)"
            ModifyArgument{
                index: 3
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Position1
                ArgumentMap{
                    index: 3
                    metaName: "%item"
                }
                Text{content: "if (itemAt(row, role)!=null)\n"+
                              "    throw new IllegalArgumentException(\"Cell in form layout is already occupied\");\n"+
                              "QWidget widget = %item.widget();"}
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "if(widget!=null && widget.parent()==null){\n"+
                              "    QtJambi_LibraryUtilities.internal.addReferenceCount(this, io.qt.widgets.QLayout.class, \"__rcLayoutedObjects\", false, false, widget);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "addRow(QWidget*,QWidget*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%1.parent()==null"
                    action: ReferenceCount.Add
                }
            }
            ModifyArgument{
                index: 2
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%2.parent()==null"
                    action: ReferenceCount.Add
                }
            }
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%1))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");\n"+
                              "if(QWidgetItem.hasWidgetItemV2(%2))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
        }
        ModifyFunction{
            signature: "addRow(QLayout*)"
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
        }
        ModifyFunction{
            signature: "removeRow(QLayout*)"
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
        }
        ModifyFunction{
            signature: "takeRow(QLayout*)"
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
        }
        ModifyFunction{
            signature: "addRow(QWidget*,QLayout*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%1.parent()==null"
                    action: ReferenceCount.Add
                }
            }
            ModifyArgument{
                index: 2
                threadAffinity: true
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
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%1))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
        }
        ModifyFunction{
            signature: "addRow(QWidget*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%1.parent()==null"
                    action: ReferenceCount.Add
                }
            }
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%1))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
        }
        ModifyFunction{
            signature: "removeRow(QWidget*)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "takeRow(QWidget*)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "addRow(QString,QLayout*)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addRow(QString,QWidget*)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%arg.parent()==null"
                    action: ReferenceCount.Add
                }
            }
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%2))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
        }
        ModifyFunction{
            signature: "insertRow(int,QLayout*)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertRow(int,QWidget*,QLayout*)"
            threadAffinity: true
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%2))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
            ModifyArgument{
                index: 2
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%arg.parent()==null"
                    action: ReferenceCount.Add
                }
            }
            ModifyArgument{
                index: 3
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertRow(int,QWidget*,QWidget*)"
            threadAffinity: true
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 3
                    metaName: "%3"
                }
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%3))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");\n"+
                              "if(QWidgetItem.hasWidgetItemV2(%2))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
            ModifyArgument{
                index: 2
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%arg.parent()==null"
                    action: ReferenceCount.Add
                }
            }
            ModifyArgument{
                index: 3
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%arg.parent()==null"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "insertRow(int,QWidget*)"
            threadAffinity: true
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%2))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
            ModifyArgument{
                index: 2
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%arg.parent()==null"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "insertRow(int,QString,QLayout*)"
            threadAffinity: true
            ModifyArgument{
                index: 3
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertRow(int,QString,QWidget*)"
            threadAffinity: true
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 3
                    metaName: "%3"
                }
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%3))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
            ModifyArgument{
                index: 3
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%arg.parent()==null"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "setRowVisible(QWidget *, bool)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: [6, 4]
        }
        ModifyFunction{
            signature: "setRowVisible(QLayout *, bool)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: [6, 4]
        }
        ModifyFunction{
            signature: "setLayout(int,QFormLayout::ItemRole,QLayout*)"
            threadAffinity: true
            ModifyArgument{
                index: 3
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setWidget(int,QFormLayout::ItemRole,QWidget*)"
            threadAffinity: true
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 3
                    metaName: "%3"
                }
                Text{content: "if(QWidgetItem.hasWidgetItemV2(%3))\n"+
                              "    throw new IllegalArgumentException(\"Widget is already a layout member.\");"}
            }
            ModifyArgument{
                index: 3
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcLayoutedObjects"
                    declareVariable: "QLayout"
                    condition: "%arg.parent()==null"
                    action: ReferenceCount.Add
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QFormLayout___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsView"
        ExtraIncludes{
            Include{
                fileName: "QPainterPath"
                location: Include.Global
            }
            Include{
                fileName: "QVarLengthArray"
                location: Include.Global
            }
            Include{
                fileName: "QtJambiCore/hashes.h"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "fitInView(const QGraphicsItem *, Qt::AspectRatioMode)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "setupViewport(QWidget *)"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "setScene(QGraphicsScene*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcScene"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setupViewport(QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "drawBackground(QPainter*,QRectF)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "drawForeground(QPainter*,QRectF)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "drawItems(QPainter*,int,QGraphicsItem**,const QStyleOptionGraphicsItem*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 3
                AsArray{
                    lengthParameter: 2
                    noOffset: true
                }
            }
            ModifyArgument{
                index: 4
                AsArray{
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "itemAt(QPoint)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "itemAt(int,int)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QInputDialog"
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QInputDialog__java_"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "open(QObject*,const char*)"
            threadAffinity: true
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: "if(!member.startsWith(\"1\") || !member.startsWith(\"2\")) member = '1'+member;"}
            }
        }
        ModifyFunction{
            signature: "getInt(QWidget*,const QString &,const QString &,int,int,int,int,bool*,Qt::WindowFlags)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.@Nullable Integer"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? QtJambiAPI::toJavaIntegerObject(%env, %in) : nullptr;"}
                }
            }
            ModifyArgument{
                index: 8
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok(false);\n"+
                                  "bool* %out = &ok;"}
                }
            }
        }
        ModifyFunction{
            signature: "getDouble(QWidget *, const QString &, const QString &, double, double, double, int, bool *, Qt::WindowFlags)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.@Nullable Double"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? QtJambiAPI::toJavaDoubleObject(%env, %in) : nullptr;"}
                }
            }
            ModifyArgument{
                index: 8
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok(false);\n"+
                                  "bool* %out = &ok;"}
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "getDouble(QWidget *, const QString &, const QString &, double, double, double, int, bool *, Qt::WindowFlags, double)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.@Nullable Double"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? QtJambiAPI::toJavaDoubleObject(%env, %in) : nullptr;"}
                }
            }
            ModifyArgument{
                index: 8
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok(false);\n"+
                                  "bool* %out = &ok;"}
                }
            }
        }
        ModifyFunction{
            signature: "getItem(QWidget*,QString,QString,QStringList,int,bool,bool*,Qt::WindowFlags,Qt::InputMethodHints)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.@Nullable String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? qtjambi_cast<jstring>(%env, %in) : nullptr;"}
                }
            }
            ModifyArgument{
                index: 7
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok(false);\n"+
                                  "bool* %out = &ok;"}
                }
            }
        }
        ModifyFunction{
            signature: "getText(QWidget *, const QString &, const QString &, QLineEdit::EchoMode, const QString &, bool *, Qt::WindowFlags,Qt::InputMethodHints)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.@Nullable String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? qtjambi_cast<jstring>(%env, %in) : nullptr;"}
                }
            }
            ModifyArgument{
                index: 6
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok(false);\n"+
                                  "bool* %out = &ok;"}
                }
            }
        }
        ModifyFunction{
            signature: "getMultiLineText(QWidget *, const QString &, const QString &, const QString &, bool *, Qt::WindowFlags,Qt::InputMethodHints)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.@Nullable String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? qtjambi_cast<jstring>(%env, %in) : nullptr;"}
                }
            }
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok(false);\n"+
                                  "bool* %out = &ok;"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsScene"
        isValueOwner: true
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
            Include{
                fileName: "QtJambiCore/hashes.h"
                location: Include.Global
            }
            Include{
                fileName: "QVarLengthArray"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "setFont(QFont)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setPalette(QPalette)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setFocus(Qt::FocusReason)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "contextMenuEvent(QGraphicsSceneContextMenuEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dragEnterEvent(QGraphicsSceneDragDropEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dragLeaveEvent(QGraphicsSceneDragDropEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dragMoveEvent(QGraphicsSceneDragDropEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "drawBackground(QPainter*,QRectF)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "drawForeground(QPainter*,QRectF)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "drawItems(QPainter*,int,QGraphicsItem**,const QStyleOptionGraphicsItem*,QWidget*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dropEvent(QGraphicsSceneDragDropEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "focusInEvent(QFocusEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "focusOutEvent(QFocusEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "helpEvent(QGraphicsSceneHelpEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "inputMethodEvent(QInputMethodEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "keyPressEvent(QKeyEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "keyReleaseEvent(QKeyEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseMoveEvent(QGraphicsSceneMouseEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mousePressEvent(QGraphicsSceneMouseEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseReleaseEvent(QGraphicsSceneMouseEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "wheelEvent(QGraphicsSceneWheelEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "setActiveWindow(QGraphicsWidget*)"
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
            signature: "setStyle(QStyle*)"
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
            signature: "sendEvent(QGraphicsItem *, QEvent *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
        }
        ModifyFunction{
            signature: "setActivePanel(QGraphicsItem*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "addItem(QGraphicsItem *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "addEllipse(qreal,qreal,qreal,qreal, const QPen &, const QBrush &)"
            threadAffinity: true
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            ModifyArgument{
                index: 5
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 6
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "addEllipse(const QRectF &, const QPen &, const QBrush &)"
            threadAffinity: true
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 3
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "addLine(qreal,qreal,qreal,qreal, const QPen &)"
            threadAffinity: true
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            ModifyArgument{
                index: 5
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "addLine(const QLineF &, const QPen &)"
            threadAffinity: true
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "addPath(const QPainterPath &, const QPen &, const QBrush &)"
            threadAffinity: true
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 3
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "addPixmap(const QPixmap &)"
            threadAffinity: true
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "addPolygon(const QPolygonF &, const QPen &, const QBrush &)"
            threadAffinity: true
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 3
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "addRect(qreal,qreal,qreal,qreal, const QPen &, const QBrush &)"
            threadAffinity: true
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            ModifyArgument{
                index: 5
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 6
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "addRect(const QRectF &, const QPen &, const QBrush &)"
            threadAffinity: true
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 3
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "addSimpleText(const QString &, const QFont &)"
            threadAffinity: true
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "addText(const QString &, const QFont &)"
            threadAffinity: true
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "addWidget(QWidget*,Qt::WindowFlags)"
            threadAffinity: true
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
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
            signature: "activePanel()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "focusItem()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "mouseGrabberItem()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "itemAt(QPointF,QTransform)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "itemAt(qreal,qreal,QTransform)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeItem(QGraphicsItem*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "setFocusItem(QGraphicsItem*,Qt::FocusReason)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcFocusItem"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "createItemGroup(const QList<QGraphicsItem *> &)"
            threadAffinity: true
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
            signature: "drawItems(QPainter*,int,QGraphicsItem**,const QStyleOptionGraphicsItem*,QWidget*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 3
                AsArray{
                    lengthParameter: 2
                    noOffset: true
                }
            }
            ModifyArgument{
                index: 4
                AsArray{
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
    }
    
    ObjectType{
        name: "QCalendarWidget"
        ExtraIncludes{
            Include{
                fileName: "QTextCharFormat"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "paintCell(QPainter*,QRect,QDate)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QTreeWidget"
        implementing: "Iterable<io.qt.widgets.QTreeWidgetItem>"
        ModifyFunction{
            signature: "setSelectionModel(QItemSelectionModel*)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcItemSelectionModel"
                    declareVariable: "QAbstractItemView"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "removeItemWidget(QTreeWidgetItem*,int)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1.treeWidget() == null) QtJambi_LibraryUtilities.internal.setDefaultOwnership(%1);"}
            }
        }
        ModifyFunction{
            signature: "mimeData(const QList<QTreeWidgetItem*>)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dropMimeData(QTreeWidgetItem*,int,const QMimeData*,Qt::DropAction)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "addTopLevelItem(QTreeWidgetItem *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "takeTopLevelItem(int)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "addTopLevelItems(const QList<QTreeWidgetItem*> &)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertTopLevelItem(int, QTreeWidgetItem *)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertTopLevelItems(int, const QList<QTreeWidgetItem*> &)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setHeaderItem(QTreeWidgetItem *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "takeTopLevelItem(int)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "setCurrentItem(QTreeWidgetItem*,int,QItemSelectionModel::SelectionFlags)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setFirstItemColumnSpanned(const QTreeWidgetItem*,bool)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setCurrentItem(QTreeWidgetItem*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setCurrentItem(QTreeWidgetItem*,int)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setItemExpanded(const QTreeWidgetItem*,bool)"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "isItemExpanded(const QTreeWidgetItem*)const"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "setItemHidden(const QTreeWidgetItem*,bool)"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "isItemHidden(const QTreeWidgetItem*)const"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "isItemSelected(const QTreeWidgetItem*)const"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "setItemSelected(const QTreeWidgetItem*,bool)"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "setItemWidget(QTreeWidgetItem*,int,QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: 3
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setModel(QAbstractItemModel*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcItemModel"
                    declareVariable: "QAbstractItemView"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "items(const QMimeData*)const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "indexFromItem(QTreeWidgetItem *, int) const"
            remove: RemoveFlag.All
            since: [5, 7]
            until: 5
        }
        InjectCode{
            InsertTemplate{
                name: "core.to_iterator"
                Replace{
                    from: "%ITERATOR_TYPE"
                    to: "io.qt.widgets.QTreeWidgetItemIterator"
                }
            }
        }
    }
    
    ObjectType{
        name: "QAbstractItemDelegate"
        ModifyFunction{
            signature: "setEditorData(QWidget*,QModelIndex)const"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setModelData(QWidget*,QAbstractItemModel*,QModelIndex)const"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "paint(QPainter*,QStyleOptionViewItem,QModelIndex)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "editorEvent(QEvent*,QAbstractItemModel*,QStyleOptionViewItem,QModelIndex)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "helpEvent(QHelpEvent*, QAbstractItemView*, const QStyleOptionViewItem &, const QModelIndex &)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "elidedText(QFontMetrics, int, Qt::TextElideMode, QString)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &) const"
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
        name: "QTableWidgetItem"
        threadAffinity: "%1->tableWidget()"
        ModifyFunction{
            signature: "clone() const"
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
        ModifyFunction{
            signature: "QTableWidgetItem(const QTableWidgetItem &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QTableWidgetItem(const QIcon &, const QString &, int)"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
            }
        }
        ModifyFunction{
            signature: "QTableWidgetItem(const QString &, int)"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
            }
        }
        ModifyFunction{
            signature: "QTableWidgetItem(int)"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
            }
        }
        ModifyFunction{
            signature: "clone() const"
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
        ModifyFunction{
            signature: "backgroundColor()const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setBackgroundColor(QColor)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setTextColor(QColor)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "textColor()const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "type()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator<(QTableWidgetItem)const"
            access: Modification.Private
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "read(QDataStream&)"
            access: Modification.Protected
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "write(QDataStream&)const"
            access: Modification.Protected
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QListWidgetItem"
        threadAffinity: "%1->listWidget()"
        ModifyFunction{
            signature: "QListWidgetItem(const QListWidgetItem &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QListWidgetItem(QListWidget *, int)"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QListWidgetItem(const QString &, QListWidget *, int)"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if (%2 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QListWidgetItem(const QIcon &, const QString &, QListWidget *, int)"
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 3
                    metaName: "%3"
                }
                Text{content: "if (%3 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "clone() const"
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
        ModifyFunction{
            signature: "backgroundColor()const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setBackgroundColor(QColor)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setTextColor(QColor)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "textColor()const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "type()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QTreeWidgetItem"
        implementing: "Iterable<io.qt.widgets.QTreeWidgetItem>"
        threadAffinity: "%1->treeWidget() ? %1->treeWidget()->model() : nullptr"

        Rejection{enumName: "ItemType"}
        Rejection{functionName: "treeModel"}

        EnumType{
            name: "ChildIndicatorPolicy"
        }
        ModifyFunction{
            signature: "QTreeWidgetItem(const QTreeWidgetItem &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "type()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QTreeWidgetItem(QTreeWidget *,int)"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QTreeWidgetItem(QTreeWidget *,const QStringList &,int)"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QTreeWidgetItem(QTreeWidget *,QTreeWidgetItem *,int)"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QTreeWidgetItem(QTreeWidgetItem *,int)"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QTreeWidgetItem(QTreeWidgetItem *,const QStringList &,int)"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QTreeWidgetItem(const QStringList &,int)"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
            }
        }
        ModifyFunction{
            signature: "QTreeWidgetItem(QTreeWidgetItem *,QTreeWidgetItem *,int)"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QTreeWidgetItem(int)"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
            }
        }
        ModifyFunction{
            signature: "clone() const"
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
        ModifyFunction{
            signature: "addChild(QTreeWidgetItem *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "addChildren(const QList<QTreeWidgetItem*> &)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertChild(int, QTreeWidgetItem *)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertChildren(int, const QList<QTreeWidgetItem*> &)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "removeChild(QTreeWidgetItem*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "takeChild(int)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "takeChildren()"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "backgroundColor(int)const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setBackgroundColor(int, QColor)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setTextColor(int, QColor)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "textColor(int)const"
            remove: RemoveFlag.All
            until: 5
        }
        InjectCode{
            InsertTemplate{
                name: "core.to_iterator"
                Replace{
                    from: "%ITERATOR_TYPE"
                    to: "io.qt.widgets.QTreeWidgetItemIterator"
                }
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsTextItem"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
            Include{
                fileName: "QTextCursor"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "setDocument(QTextDocument*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcDocument"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "QGraphicsTextItem(QGraphicsItem*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "QGraphicsTextItem(const QString &,QGraphicsItem*)"
            ModifyArgument{
                index: 2
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if (%2 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
    }
    
    ObjectType{
        name: "QCompleter"
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcModel"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setPopup(QAbstractItemView *)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setWidget(QWidget *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWidget"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QListWidget"
        ModifyFunction{
            signature: "mimeData(const QList<QListWidgetItem *>)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "addItem(QListWidgetItem *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertItem(int, QListWidgetItem *)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setCurrentItem(QListWidgetItem*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setCurrentItem(QListWidgetItem*,QItemSelectionModel::SelectionFlags)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setItemHidden(const QListWidgetItem*,bool)"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "isItemHidden(const QListWidgetItem*)const"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "setItemSelected(const QListWidgetItem*,bool)"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "isItemSelected(const QListWidgetItem*)const"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "takeItem(int)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "setItemWidget(QListWidgetItem*,QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeItemWidget(QListWidgetItem*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setModel(QAbstractItemModel*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcItemModel"
                    declareVariable: "QAbstractItemView"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setSelectionModel(QItemSelectionModel *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcItemSelectionModel"
                    declareVariable: "QAbstractItemView"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "indexFromItem(QListWidgetItem *)const"
            remove: RemoveFlag.All
            since: [5, 11]
            until: 5
        }
        ModifyFunction{
            signature: "currentItem()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "item(int)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "itemAt(QPoint)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "itemAt(int,int)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "itemFromIndex(QModelIndex)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QWidget"

        Rejection{functionName: "create"}
        Rejection{functionName: "destroy"}
        Rejection{functionName: "find"}
        Rejection{functionName: "handle"}
        Rejection{functionName: "styleChange"}
        Rejection{functionName: "internalWinId"}
        EnumType{
            name: "RenderFlag"
        }
        ExtraIncludes{
            Include{
                fileName: "QIcon"
                location: Include.Global
            }
            Include{
                fileName: "QMessageBox"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "hasEditFocus() const"
            ppCondition: "defined(QT_KEYPAD_NAVIGATION)"
        }
        ModifyFunction{
            signature: "setEditFocus(bool)"
            ppCondition: "defined(QT_KEYPAD_NAVIGATION)"
        }
        ModifyFunction{
            signature: "setLayoutDirection(Qt::LayoutDirection)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "mouseGrabber()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "keyboardGrabber()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setTabOrder(QWidget *, QWidget *)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setFocusProxy(QWidget *)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setContextMenuPolicy(Qt::ContextMenuPolicy)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "grabMouse(QCursor)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "grabMouse()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "releaseMouse()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "grabKeyboard()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setUpdatesEnabled(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "releaseKeyboard()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "grabShortcut(QKeySequence, Qt::ShortcutContext)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "releaseShortcut(int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setShortcutEnabled(int, bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setShortcutAutoRepeat(int, bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setFocus()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setFocus(Qt::FocusReason)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setFocusPolicy(Qt::FocusPolicy)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setLocale(QLocale)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "createWinId()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setMouseTracking(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setTabletTracking(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setMask(QBitmap)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setMask(QRegion)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setWindowIconText(QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setWindowRole(QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setWindowFilePath(QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setWindowOpacity(qreal)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setStyleSheet(QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setAccessibleDescription(QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setAccessibleName(QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setWhatsThis(QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setStatusTip(QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setToolTip(QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setToolTipDuration(int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setWindowIcon(QIcon)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setCursor(QCursor)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setFont(QFont)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setForegroundRole(QPalette::ColorRole)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setBackgroundRole(QPalette::ColorRole)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setPalette(QPalette)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setStyle(QStyle *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setEnabled(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setDisabled(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setMinimumSize(QSize)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setMaximumSize(QSize)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setMinimumSize(int,int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setMaximumSize(int,int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setMinimumWidth(int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setMaximumWidth(int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setMinimumHeight(int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setMaximumHeight(int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setFixedHeight(int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setFixedWidth(int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setSizeIncrement(QSize)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setBaseSize(QSize)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setFixedSize(QSize)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setSizeIncrement(int,int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setBaseSize(int,int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setFixedSize(int,int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "scroll(int, int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "scroll(int, int, QRect)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setContentsMargins(int, int, int, int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setContentsMargins(QMargins)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setSizePolicy(QSizePolicy)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setSizePolicy(QSizePolicy::Policy,QSizePolicy::Policy)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "close()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "stackUnder(QWidget*)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "ensurePolished()const"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setParent(QWidget *)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setParent(QWidget *, Qt::WindowFlags)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setAttribute(Qt::WidgetAttribute, bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setWindowModified(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setToolTip(QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "raise()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "lower()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setMask(QBitmap)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setMask(QRegion)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setVisible(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "resize(int,int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "resize(QSize)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "move(int,int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "move(QPoint)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "show()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "hide()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "update()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "repaint()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "repaint(QRect)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "repaint(QRegion)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "repaint(int,int,int,int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setGeometry(int,int,int,int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setGeometry(QRect)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "saveGeometry()const"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "restoreGeometry(QByteArray)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "adjustSize()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "update(QRect)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "update(QRegion)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setHidden(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "showFullScreen()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "showMaximized()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "showMinimized()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "showNormal()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "focusNextPrevChild(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "focusNextChild()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "focusPreviousChild()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setWindowFlag(Qt::WindowType, bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setWindowFlags(Qt::WindowFlags)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setWindowState(Qt::WindowStates)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "overrideWindowState(Qt::WindowStates)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "createWindowContainer(QWindow *, QWidget *, Qt::WindowFlags)"
            threadAffinity: Affinity.UI
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
            signature: "devType() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "nativeEvent(const QByteArray &, void *, long *)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "nativeEvent(const QByteArray &, void *, qintptr *)"
            remove: RemoveFlag.All
            since: 6
        }
        ModifyFunction{
            signature: "actionEvent(QActionEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "changeEvent(QEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "closeEvent(QCloseEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "contextMenuEvent(QContextMenuEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dragEnterEvent(QDragEnterEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dragLeaveEvent(QDragLeaveEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dragMoveEvent(QDragMoveEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "dropEvent(QDropEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "enterEvent(QEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            until: 5
        }
        ModifyFunction{
            signature: "enterEvent(QEnterEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            since: 6
        }
        ModifyFunction{
            signature: "focusInEvent(QFocusEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "focusOutEvent(QFocusEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "hideEvent(QHideEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "inputMethodEvent(QInputMethodEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "keyPressEvent(QKeyEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "keyReleaseEvent(QKeyEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "leaveEvent(QEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseDoubleClickEvent(QMouseEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseMoveEvent(QMouseEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mousePressEvent(QMouseEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseReleaseEvent(QMouseEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "moveEvent(QMoveEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "paintEvent(QPaintEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "resizeEvent(QResizeEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "showEvent(QShowEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "tabletEvent(QTabletEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "wheelEvent(QWheelEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "setGraphicsEffect(QGraphicsEffect*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcGraphicsEffect"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "render(QPaintDevice *, const QPoint &, const QRegion &, QWidget::RenderFlags)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 4
                ReplaceDefaultExpression{
                    expression: "RenderFlag.DrawWindowBackground, RenderFlag.DrawChildren"
                }
            }
        }
        ModifyFunction{
            signature: "render(QPainter *, const QPoint &, const QRegion &, QWidget::RenderFlags)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 4
                ReplaceDefaultExpression{
                    expression: "RenderFlag.DrawWindowBackground, RenderFlag.DrawChildren"
                }
            }
            ModifyArgument{
                index: 2
                RemoveDefaultExpression{
                }
            }
        }
        ModifyFunction{
            signature: "setFocusProxy(QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcFocusProxy"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setScreen(QScreen*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "setLayout(QLayout*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setParent(QWidget*,Qt::WindowFlags)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setParent(QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setStyle(QStyle*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcStyle"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setTabOrder(QWidget*,QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addAction(QAction *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcActions"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "insertAction(QAction *, QAction *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcActions"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "addActions(const QList<QAction *> &)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcActions"
                    action: ReferenceCount.AddAll
                }
            }
        }
        ModifyFunction{
            signature: "insertActions(QAction *, const QList<QAction *> &)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcActions"
                    action: ReferenceCount.AddAll
                }
            }
        }
        ModifyFunction{
            signature: "removeAction(QAction *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcActions"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "addAction<Args...,>(QIcon,QString,QKeySequence,Args&&)"
            remove: RemoveFlag.All
            since: 6.3
        }
        ModifyFunction{
            signature: "addAction<Args...,>(QString,QKeySequence,Args&&)"
            remove: RemoveFlag.All
            since: 6.3
        }
        ModifyFunction{
            signature: "addAction<Args...,>(QIcon,QString,Args&&)"
            remove: RemoveFlag.All
            since: 6.3
        }
        ModifyFunction{
            signature: "addAction<Args...,>(QString,Args&&)"
            remove: RemoveFlag.All
            since: 6.3
        }
        ModifyFunction{
            signature: "addAction(QString)"
            threadAffinity: Affinity.UI
            since: [6, 3]
        }
        ModifyFunction{
            signature: "addAction(QString,QKeySequence)"
            threadAffinity: Affinity.UI
            since: [6, 3]
        }
        ModifyFunction{
            signature: "addAction(QIcon,QString)"
            threadAffinity: Affinity.UI
            since: [6, 3]
        }
        ModifyFunction{
            signature: "addAction(QIcon,QString,QKeySequence)"
            threadAffinity: Affinity.UI
            since: [6, 3]
        }
        ModifyFunction{
            signature: "addAction(QIcon,QString,const QObject*,const char*,Qt::ConnectionType)"
            remove: RemoveFlag.All
            since: [6, 3]
        }
        ModifyFunction{
            signature: "addAction(QIcon,QString,QKeySequence,const QObject*,const char*,Qt::ConnectionType)"
            remove: RemoveFlag.All
            since: [6, 3]
        }
        ModifyFunction{
            signature: "addAction(QString,const QObject*,const char*,Qt::ConnectionType)"
            remove: RemoveFlag.All
            since: [6, 3]
        }
        ModifyFunction{
            signature: "addAction(QString,QKeySequence,const QObject*,const char*,Qt::ConnectionType)"
            remove: RemoveFlag.All
            since: [6, 3]
        }
        ExtraIncludes{
            Include{
                fileName: "io.qt.core.*"
                location: Include.Java
            }
            Include{
                fileName: "io.qt.gui.*"
                location: Include.Java
            }
            since: [6, 3]
        }
        InjectCode{
            since: [6, 3]
            InsertTemplate{
                name: "widget.addAction_with_shortcut"
                Replace{
                    from: "%EXTRA"
                    to: ""
                }
                Replace{
                    from: "%CALL_EXTRA"
                    to: ""
                }
            }
            InsertTemplate{
                name: "widget.addAction_with_shortcut"
                Replace{
                    from: "%EXTRA"
                    to: "QIcon icon,"
                }
                Replace{
                    from: "%CALL_EXTRA"
                    to: "icon,"
                }
            }
        }
        ModifyFunction{
            signature: "paintEvent(QPaintEvent*)"
            threadAffinity: Affinity.UI
            isPaintMethod: true
        }
        ModifyFunction{
            signature: "setWindowTitle(const QString &)"
            threadAffinity: Affinity.UI
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (QMessageBox* box = dynamic_cast<QMessageBox*>(__qt_this)) {\n"+
                              "    box->setWindowTitle(__qt_%1);\n"+
                              "    return;\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "setWindowModality(Qt::WindowModality)"
            threadAffinity: Affinity.UI
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (QMessageBox* box = dynamic_cast<QMessageBox*>(__qt_this)) {\n"+
                              "    box->setWindowModality(__qt_%1);\n"+
                              "    return;\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "grabGesture(Qt::GestureType, Qt::GestureFlags)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "new io.qt.core.Qt.GestureFlag[0]"
                }
            }
        }
        ModifyFunction{
            signature: "grab(const QRect &)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "new io.qt.core.QRect(new io.qt.core.QPoint(0, 0), new io.qt.core.QSize(-1, -1))"
                }
            }
        }
        ModifyFunction{
            signature: "getContentsMargins(int*,int*,int*,int*)const"
            threadAffinity: Affinity.UI
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(%1.length<4) throw new IllegalArgumentException(\"An array of length 4 expected.\");"}
            }
            ModifyArgument{
                index: 1
                rename: "margins"
                ReplaceType{
                    modifiedType: "int @NonNull[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 4;\n"+
                                  "int* %out = qtjambi_array_cast<int*>(%env, %scope, %in, %out_size);"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int* %out = __qt_%1+1;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int* %out = __qt_%1+2;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int* %out = __qt_%1+3;"}
                }
            }
            until: 5
        }
    }
    
    ObjectType{
        name: "QMessageBox"
        ModifyFunction{
            signature: "setWindowTitle(const QString &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setWindowModality(Qt::WindowModality)"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QPixmap"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "addButton(QAbstractButton*,QMessageBox::ButtonRole)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setCheckBox(QCheckBox*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "removeButton(QAbstractButton*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setDefaultButton(QPushButton*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setEscapeButton(QAbstractButton*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "QMessageBox(QString,QString,QMessageBox::Icon,int,int,int,QWidget*,Qt::WindowFlags)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "buttonText(int)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setButtonText(int, QString)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "standardIcon(QMessageBox::Icon)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "critical(QWidget*,QString,QString,int,int,int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "critical(QWidget*,QString,QString,QString,QString,QString,int,int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "information(QWidget*,QString,QString,int,int,int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "information(QWidget*,QString,QString,QString,QString,QString,int,int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "question(QWidget*, QString, QString, int, int, int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "question(QWidget*, QString, QString, QString, QString, QString, int, int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "warning(QWidget*, QString, QString, int, int, int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "warning(QWidget*, QString, QString, QString, QString, QString, int, int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "open(QObject*,const char*)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "information(QWidget *, QString, QString, QMessageBox::StandardButtons, QMessageBox::StandardButton)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "question(QWidget *, QString, QString, QMessageBox::StandardButtons, QMessageBox::StandardButton)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "warning(QWidget *, QString, QString, QMessageBox::StandardButtons, QMessageBox::StandardButton)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "critical(QWidget *, QString, QString, QMessageBox::StandardButtons, QMessageBox::StandardButton)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "about(QWidget *, QString, QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "aboutQt(QWidget *, QString)"
            threadAffinity: Affinity.UI
        }
    }
    
    ObjectType{
        name: "QAbstractSpinBox"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }

        EnumType{
            name: "ButtonSymbols"
        }

        EnumType{
            name: "CorrectionMode"
        }

        EnumType{
            name: "StepType"
        }

        EnumType{
            name: "StepEnabledFlag"
        }
        ModifyFunction{
            signature: "initStyleOption(QStyleOptionSpinBox*)const"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "setReadOnly(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setLineEdit(QLineEdit*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "validate(QString&,int&)const"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.gui.QValidator$QValidationData"
                }
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Shell
                    InsertTemplate{
                        name: "gui.convert_validationdata_to_java"
                        Replace{
                            from: "%STRING"
                            to: "%1"
                        }
                        Replace{
                            from: "%POS"
                            to: "%2"
                        }
                    }
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "gui.convert_validationdata_to_string"
                    }
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "// nothing needed"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "gui.convert_validationdata_to_pos"
                        Replace{
                            from: "%VALIDATIONDATA"
                            to: "%1"
                        }
                    }
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                InsertTemplate{
                    name: "gui.cleanup_validationdata_from_java"
                }
            }
            InjectCode{
                target: CodeClass.Shell
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%STRING"
                }
                ArgumentMap{
                    index: 2
                    metaName: "%POS"
                }
                InsertTemplate{
                    name: "gui.cleanup_validationdata_to_java"
                }
            }
        }
        InjectCode{
            target: CodeClass.Java
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QAbstractSpinBox___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    
    ObjectType{
        name: "QTabWidget"
        ModifyFunction{
            signature: "addTab(QWidget*,QIcon,QString)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "addTab(QWidget*,QString)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertTab(int,QWidget*,QString)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertTab(int,QWidget*,QIcon,QString)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setCornerWidget(QWidget*,Qt::Corner)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setCurrentWidget(QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setTabBar(QTabBar*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QDateTimeEdit"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "QDateTimeEdit(const QVariant &, QVariant::Type, QWidget *)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setCalendarWidget(QCalendarWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QSlider"
    }
    
    ObjectType{
        name: "QProgressDialog"
        ModifyFunction{
            signature: "setBar(QProgressBar*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setCancelButton(QPushButton*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setLabel(QLabel*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "forceShow()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setValue(int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setMinimumDuration(int)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setCancelButtonText(QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setLabelText(QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "open(QObject*,const char*)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QLabel"
        ModifyFunction{
            signature: "setBuddy(QWidget *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcBuddy"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setMovie(QMovie *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMovie"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "picture()const"
            remove: RemoveFlag.All
            since: [5, 15]
        }
        ModifyFunction{
            signature: "pixmap()const"
            remove: RemoveFlag.All
            since: [5, 15]
        }
        ModifyFunction{
            signature: "picture(Qt::ReturnByValueConstant)const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Qt::ReturnByValueConstant %out = Qt::ReturnByValue;"}
                }
            }
            since: [5, 15]
        }
        ModifyFunction{
            signature: "pixmap(Qt::ReturnByValueConstant)const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Qt::ReturnByValueConstant %out = Qt::ReturnByValue;"}
                }
            }
            since: [5, 15]
        }
    }
    
    ObjectType{
        name: "QFileDialog"
        FunctionalType{
            name: "FileContentHandler"
            generate: false
            using: "std::function<void(const QString&,const QByteArray&)>"
        }
        ExtraIncludes{
            Include{
                fileName: "QUrl"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "setProxyModel(QAbstractProxyModel*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QFileDialog__java_"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "open(QObject*,const char*)"
            threadAffinity: true
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: "if(!member.startsWith(\"1\") || !member.startsWith(\"2\")) member = '1'+member;"}
            }
        }
        ModifyFunction{
            signature: "getExistingDirectory(QWidget*,QString,QString,QFileDialog::Options)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = %in.isEmpty() ? nullptr : qtjambi_cast<jstring>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "getExistingDirectoryUrl(QWidget*,QString,QUrl,QFileDialog::Options,QStringList)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = %in.isEmpty() ? nullptr : qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "getOpenFileName(QWidget*,QString,QString,QString,QString*,QFileDialog::Options)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.widgets.QFileDialog$@NonNull Result<java.lang.@NonNull String>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "if(!%in.isEmpty()){\n"+
                                  "    %out = Java::QtWidgets::QFileDialog$Result::newInstance(%env, qtjambi_cast<jstring>(%env, %in), qtjambi_cast<jobject>(%env, %5));\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "getOpenFileNames(QWidget*,QString,QString,QString,QString*,QFileDialog::Options)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.widgets.QFileDialog$@NonNull Result<io.qt.core.@NonNull QStringList>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "if(!%in.isEmpty()){\n"+
                                  "    %out = Java::QtWidgets::QFileDialog$Result::newInstance(%env, qtjambi_cast<jobject>(%env, %in), qtjambi_cast<jobject>(%env, %5));\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "getSaveFileName(QWidget*,QString,QString,QString,QString*,QFileDialog::Options)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.widgets.QFileDialog$@NonNull Result<java.lang.@NonNull String>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "if(!%in.isEmpty()){\n"+
                                  "    %out = Java::QtWidgets::QFileDialog$Result::newInstance(%env, qtjambi_cast<jstring>(%env, %in), qtjambi_cast<jobject>(%env, %5));\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "getOpenFileUrl(QWidget*,QString,QUrl,QString,QString*,QFileDialog::Options,QStringList)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.widgets.QFileDialog$@NonNull Result<io.qt.core.@NonNull QUrl>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "if(!%in.isEmpty()){\n"+
                                  "    %out = Java::QtWidgets::QFileDialog$Result::newInstance(%env, qtjambi_cast<jobject>(%env, %in), qtjambi_cast<jobject>(%env, %5));\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "getOpenFileUrls(QWidget*,QString,QUrl,QString,QString*,QFileDialog::Options,QStringList)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.widgets.QFileDialog$@NonNull Result<io.qt.core.@NonNull QList<io.qt.core.@NonNull QUrl>>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "if(!%in.isEmpty()){\n"+
                                  "    %out = Java::QtWidgets::QFileDialog$Result::newInstance(%env, qtjambi_cast<jobject>(%env, %in), qtjambi_cast<jobject>(%env, %5));\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "getSaveFileUrl(QWidget*,QString,QUrl,QString,QString*,QFileDialog::Options,QStringList)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.widgets.QFileDialog$@NonNull Result<io.qt.core.@NonNull QUrl>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "if(!%in.isEmpty()){\n"+
                                  "    %out = Java::QtWidgets::QFileDialog$Result::newInstance(%env, qtjambi_cast<jobject>(%env, %in), qtjambi_cast<jobject>(%env, %5));\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "getOpenFileContent(QString, std::function<void(const QString&,const QByteArray&)>)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "null"
                }
                ReplaceType{
                    modifiedType: "java.util.function.BiConsumer<@NonNull String,io.qt.core.@NonNull QByteArray>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "std::function<void(const QString&,const QByteArray&)> %out;\n"+
                                  "if(%in){\n"+
                                  "    JObjectWrapper wrapper(%env, %in);\n"+
                                  "    %out = [wrapper](const QString& value1,const QByteArray& value2){\n"+
                                  "                        if(JniEnvironment env{200}){\n"+
                                  "                            jstring _value1 = qtjambi_cast<jstring>(env, value1);\n"+
                                  "                            jobject _value2 = qtjambi_cast<jobject>(env, value2);\n"+
                                  "                            Java::Runtime::BiConsumer::accept(env, wrapper.object(), _value1, _value2);\n"+
                                  "                        }\n"+
                                  "                    };\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "setIconProvider(QAbstractFileIconProvider *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcIconProvider"
                    action: ReferenceCount.Set
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "setIconProvider(QFileIconProvider*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcIconProvider"
                    action: ReferenceCount.Set
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setItemDelegate(QAbstractItemDelegate*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcItemDelegate"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "iconProvider()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QErrorMessage"
        ModifyFunction{
            signature: "qtHandler()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "showMessage(QString)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "showMessage(QString, QString)"
            threadAffinity: Affinity.UI
        }
    }
    
    ObjectType{
        name: "QTabBar"
        ExtraIncludes{
            Include{
                fileName: "QIcon"
                location: Include.Global
            }
        }

        EnumType{
            name: "Shape"
        }

        EnumType{
            name: "SelectionBehavior"
        }

        EnumType{
            name: "ButtonPosition"
        }
        ModifyFunction{
            signature: "initStyleOption(QStyleOptionTab*,int)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            since: 6
        }
        ModifyFunction{
            signature: "setTabButton(int,QTabBar::ButtonPosition,QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 3
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QRadioButton"
    }
    
    ObjectType{
        name: "QScrollBar"
    }
    
    ObjectType{
        name: "QAbstractScrollArea"
        ModifyFunction{
            signature: "setupViewport(QWidget *)"
            access: Modification.NonFinal
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "addScrollBarWidget(QWidget*,Qt::Alignment)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setCornerWidget(QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setHorizontalScrollBar(QScrollBar*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setVerticalScrollBar(QScrollBar*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setViewport(QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setupViewport(QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "viewportEvent(QEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ExtraIncludes{
            Include{
                fileName: "io.qt.gui.*"
                location: Include.Java
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QAbstractScrollArea___"
                quoteBeforeLine: "}// class"
            }
        }
        ExtraIncludes{
            Include{
                fileName: "io.qt.gui.*"
                location: Include.Java
            }
        }
    }
    
    ObjectType{
        name: "QRubberBand"
        ModifyFunction{
            signature: "move(int,int)"
            rename: "moveRubberBand"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "move(const QPoint &)"
            rename: "moveRubberBand"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "resize(int,int)"
            rename: "resizeRubberBand"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "resize(const QSize &)"
            rename: "resizeRubberBand"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setGeometry(int,int,int,int)"
            rename: "setRubberBandGeometry"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setGeometry(const QRect &)"
            rename: "setRubberBandGeometry"
            threadAffinity: Affinity.UI
        }
    }
    
    ObjectType{
        name: "QTableWidget"
        ModifyFunction{
            signature: "mimeData(const QList<QTableWidgetItem*>)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "isSortingEnabled()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setSortingEnabled(bool)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setHorizontalHeaderItem(int, QTableWidgetItem *)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setItem(int, int, QTableWidgetItem *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 3
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "takeHorizontalHeaderItem(int)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "takeVerticalHeaderItem(int)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "takeItem(int,int)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "setItemPrototype(const QTableWidgetItem *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setVerticalHeaderItem(int, QTableWidgetItem *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 2
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setCellWidget(int,int,QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 3
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setCurrentItem(QTableWidgetItem*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setCurrentItem(QTableWidgetItem*,QItemSelectionModel::SelectionFlags)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setItemSelected(const QTableWidgetItem*,bool)"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "isItemSelected(const QTableWidgetItem*)const"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "setModel(QAbstractItemModel*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcItemModel"
                    declareVariable: "QAbstractItemView"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "indexFromItem(QTableWidgetItem *) const"
            remove: RemoveFlag.All
            since: [5, 11]
            until: 5
        }
    }
    
    ObjectType{
        name: "QSplitter"
        ModifyFunction{
            signature: "createHandle()"
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
            signature: "addWidget(QWidget *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertWidget(int, QWidget *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QSplitter___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "getRange(int,int*,int*)const"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint %in;\n"+
                                  "int* %out = reinterpret_cast<int*>(&%in);"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint %in;\n"+
                                  "int* %out = reinterpret_cast<int*>(&%in);"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.widgets.QSplitter$Range"
                }
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtWidgets::QSplitter$Range::newInstance(%env, %2, %3);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QGroupBox"
    }
    
    ObjectType{
        name: "QStackedWidget"
        ModifyFunction{
            signature: "addWidget(QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "insertWidget(int,QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeWidget(QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setCurrentWidget(QWidget*)"
            threadAffinity: Affinity.UI
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
        name: "QSplitterHandle"
    }
    
    ObjectType{
        name: "QDial"
    }
    
    ObjectType{
        name: "QLineEdit"
        ModifyFunction{
            signature: "setReadOnly(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "getTextMargins(int*,int*,int*,int*)const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "createStandardContextMenu()"
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
            signature: "setCompleter(QCompleter *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcCompleter"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setValidator(const QValidator *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcValidator"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "addAction(QAction *, QLineEdit::ActionPosition)"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "QLCDNumber"
    }
    
    ObjectType{
        name: "QKeySequenceEdit"
        ModifyFunction{
            signature: "event(QEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "keyPressEvent(QKeyEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "keyReleaseEvent(QKeyEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "timerEvent(QTimerEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QSplashScreen"
        ModifyFunction{
            signature: "showMessage(const QString &, int, const QColor &)"
            ModifyArgument{
                index: 3
                ReplaceDefaultExpression{
                    expression: "new io.qt.gui.QColor(io.qt.core.Qt.GlobalColor.black)"
                }
            }
        }
        ModifyFunction{
            signature: "repaint()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "drawContents(QPainter*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QDockWidget"
        ModifyFunction{
            signature: "setTitleBarWidget(QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setWidget(QWidget*)"
            threadAffinity: Affinity.UI
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
        name: "QDesktopWidget"
    }
    
    ObjectType{
        name: "QFrame"
    }
    
    ObjectType{
        name: "QSpinBox"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
    }
    
    ObjectType{
        name: "QTextBrowser"
    }
    
    ObjectType{
        name: "QDoubleSpinBox"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
    }
    
    ObjectType{
        name: "QButtonGroup"
        ModifyFunction{
            signature: "addButton(QAbstractButton *, int)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcButtons"
                    action: ReferenceCount.Add
                }
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "removeButton(QAbstractButton *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcButtons"
                    action: ReferenceCount.Take
                }
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "setId(QAbstractButton *,int)"
            threadAffinity: Affinity.UI
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
        name: "QToolBar"
        ModifyFunction{
            signature: "addAction<Func1>(QString,Func1)"
            remove: RemoveFlag.All
            until: 6.3
        }
        ModifyFunction{
            signature: "addAction<Func1>(QIcon,QString,Func1)"
            remove: RemoveFlag.All
            until: 6.3
        }
        ModifyFunction{
            signature: "addAction(QAction *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "addSeparator()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "addAction(QString)"
            threadAffinity: Affinity.UI
            until: [6, 2]
        }
        ModifyFunction{
            signature: "addAction(QIcon,QString)"
            threadAffinity: Affinity.UI
            until: [6, 2]
        }
        ModifyFunction{
            signature: "addAction(QIcon,QString,const QObject*,const char*)"
            remove: RemoveFlag.All
            until: [6, 2]
        }
        ModifyFunction{
            signature: "addAction(QString,const QObject*,const char*)"
            remove: RemoveFlag.All
            until: [6, 2]
        }
        ModifyFunction{
            signature: "addWidget(QWidget*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertWidget(QAction*,QWidget*)"
            threadAffinity: Affinity.UI
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
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertSeparator(QAction*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "io.qt.gui.*"
                location: Include.Java
            }
            until: [6, 2]
        }
        InjectCode{
            until: [6, 2]
            InsertTemplate{
                name: "gui.addAction"
                Replace{
                    from: "%POST_CALL"
                    to: ""
                }
                Replace{
                    from: "%END_EXTRA"
                    to: ""
                }
                Replace{
                    from: "%END_CALL_EXTRA"
                    to: ""
                }
                Replace{
                    from: "%EXTRA"
                    to: "QIcon icon,"
                }
                Replace{
                    from: "%CALL_EXTRA"
                    to: "icon,"
                }
                Replace{
                    from: "%INFIX_EXTRA"
                    to: ""
                }
                Replace{
                    from: "%CONNECTION_TYPE"
                    to: ""
                }
                Replace{
                    from: "%USE_CONNECTION_TYPE"
                    to: ""
                }
            }
            InsertTemplate{
                name: "gui.addAction"
                Replace{
                    from: "%POST_CALL"
                    to: ""
                }
                Replace{
                    from: "%END_EXTRA"
                    to: ""
                }
                Replace{
                    from: "%END_CALL_EXTRA"
                    to: ""
                }
                Replace{
                    from: "%EXTRA"
                    to: ""
                }
                Replace{
                    from: "%INFIX_EXTRA"
                    to: ""
                }
                Replace{
                    from: "%CALL_EXTRA"
                    to: ""
                }
                Replace{
                    from: "%CONNECTION_TYPE"
                    to: ""
                }
                Replace{
                    from: "%USE_CONNECTION_TYPE"
                    to: ""
                }
            }
        }
    }
    
    ObjectType{
        name: "QComboBox"
        ModifyFunction{
            signature: "showPopup()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setAutoCompletion(bool)"
            threadAffinity: Affinity.UI
            until: 5
        }
        ModifyFunction{
            signature: "setAutoCompletionCaseSensitivity(Qt::CaseSensitivity)"
            threadAffinity: Affinity.UI
            until: 5
        }
        ModifyFunction{
            signature: "setDuplicatesEnabled(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setFrame(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy)"
            threadAffinity: Affinity.UI
        }

        ModifyFunction{
            signature: "setCompleter(QCompleter*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{action: ReferenceCount.Ignore}
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "if(lineEdit()!=null)\n" +
                              "    QtJambi_LibraryUtilities.internal.setReferenceCount(lineEdit(), QLineEdit.class, \"__rcCompleter\", false, false, c);"}
            }
        }
        ModifyFunction{
            signature: "setValidator(const QValidator*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcValidator"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setItemDelegate(QAbstractItemDelegate *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcItemDelegate"
                    action: ReferenceCount.Set
                }
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "setView(QAbstractItemView *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setLineEdit(QLineEdit *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setModel(QAbstractItemModel *)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                NoNullPointer{
                }
                ReferenceCount{
                    variableName: "__rcModel"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "autoCompletion()const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "autoCompletionCaseSensitivity()const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setAutoCompletion(bool)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setAutoCompletionCaseSensitivity(Qt::CaseSensitivity)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "findData(const QVariant&,int,Qt::MatchFlags)const"
            ModifyArgument{
                index: 3
                ReplaceDefaultExpression{
                    expression: "io.qt.core.Qt.MatchFlag.MatchExactly, io.qt.core.Qt.MatchFlag.MatchCaseSensitive"
                }
            }
        }
        ModifyFunction{
            signature: "findText(const QString&,Qt::MatchFlags)const"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "io.qt.core.Qt.MatchFlag.MatchExactly, io.qt.core.Qt.MatchFlag.MatchCaseSensitive"
                }
            }
        }
    }
    
    ObjectType{
        name: "QTextEdit"
        ExtraIncludes{
            Include{
                fileName: "QTextCursor"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "setReadOnly(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "createMimeDataFromSelection()const"
            threadAffinity: Affinity.UI
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
            signature: "createStandardContextMenu()"
            threadAffinity: Affinity.UI
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
            signature: "createStandardContextMenu(QPoint)"
            threadAffinity: Affinity.UI
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
            signature: "setDocument(QTextDocument*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcDocument"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "insertFromMimeData(const QMimeData*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QAction"
        ModifyFunction{
            signature: "setMenu(QMenu*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcMenu"
                    action: ReferenceCount.Set
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "io.qt.gui.*"
                location: Include.Java
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QAction___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "setActionGroup(QActionGroup*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (__rcActionGroup != null) {\n"+
                              "    QtJambi_LibraryUtilities.internal.removeFromCollectionReferenceCount(__rcActionGroup, QActionGroup.class, \"__rcActions\", false, this);\n"+
                              "}\n"+
                              "__rcActionGroup = %1;\n"+
                              "if (__rcActionGroup != null) {\n"+
                              "    QtJambi_LibraryUtilities.internal.addReferenceCount(__rcActionGroup, QActionGroup.class, \"__rcActions\", false, false, this);\n"+
                              "}"}
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QApplication"
        EnumType{
            name: "ColorSpec"
        }

        Rejection{functionName: "compressEvent"}
        Rejection{
            functionName: "nativeInterface"
            since: [6, 2]
        }
        ExtraIncludes{
            Include{
                fileName: "QBasicTimer"
                location: Include.Global
            }
            Include{
                fileName: "QFont"
                location: Include.Global
            }
            Include{
                fileName: "QFontMetrics"
                location: Include.Global
            }
            Include{
                fileName: "QPalette"
                location: Include.Global
            }
            Include{
                fileName: "QIcon"
                location: Include.Global
            }
            Include{
                fileName: "QLocale"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "navigationMode()"
            ppCondition: "defined(QT_KEYPAD_NAVIGATION)"
        }
        ModifyFunction{
            signature: "setNavigationMode(Qt::NavigationMode)"
            ppCondition: "defined(QT_KEYPAD_NAVIGATION)"
        }
        ModifyFunction{
            signature: "keypadNavigationEnabled()"
            ppCondition: "defined(QT_KEYPAD_NAVIGATION)"
            until: [5, 15]
        }
        ModifyFunction{
            signature: "setKeypadNavigationEnabled(bool)"
            ppCondition: "defined(QT_KEYPAD_NAVIGATION)"
            until: [5, 15]
        }
        ModifyFunction{
            signature: "setActiveWindow(QWidget*)"
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
            signature: "setStyle(QStyle*)"
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
            signature: "topLevelAt(const QPoint &)"
            Rename{
                to: "topLevelWidgetAt"
            }
        }
        ModifyFunction{
            signature: "topLevelAt(int,int)"
            Rename{
                to: "topLevelWidgetAt"
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QApplication___"
                quoteBeforeLine: "}// class"
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QtCore/private/qthread_p.h"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/CoreAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QApplication(int &, char **, int)"
            Access{
                modifiers: Modification.Protected
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "if(!Java::QtCore::QCoreApplication::__qt_isInitializing(%env)){\n"+
                              "    Java::Runtime::IllegalAccessError::throwNew(%env, \"Not allowed to instantiate QApplication. Please use QGuiApplication.initialize() instead.\" QTJAMBI_STACKTRACEINFO );\n"+
                              "    return;\n"+
                              "}"}
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "// nothing to do"}
                }
            }
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "java.lang.@Nullable String @Nullable[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "std::unique_ptr<ApplicationData> applicationData(new ApplicationData(%env, jobjectArray(%in)));\n"+
                                  "char** %out = applicationData->chars();\n"+
                                  "int& __qt_%1 = applicationData->size();"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                Text{content: "applicationData->update(%env);\n"+
                              "QTJAMBI_SET_OBJECTUSERDATA(ApplicationData, __qt_this, applicationData.release());"}
            }
        }
        ModifyFunction{
            signature: "exec()"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "QCoreApplication* instance = QCoreApplication::instance();\n"+
                              "if (!instance)\n"+
                              "    JavaException::raiseRuntimeException(%env, \"QApplication has not been initialized with QApplication.initialize()\" QTJAMBI_STACKTRACEINFO );\n"+
                              "else if(instance->thread()!=QThread::currentThread())\n"+
                              "    JavaException::raiseRuntimeException(%env, \"exec() must be called from the main thread.\" QTJAMBI_STACKTRACEINFO );\n"+
                              "else if(QThreadData::get2(instance->thread())->eventLoops.size()>0)\n"+
                              "    JavaException::raiseRuntimeException(%env, \"The event loop is already running.\" QTJAMBI_STACKTRACEINFO );"}
            }
        }
        Template{
            name: "gui.application_char_pointer_function"
            Text{content: "public static %RETURN_TYPE %FUNCTION_NAME(%PRE_ARGUMENTS %COMMA QByteArray className) {\n"+
                          "    return %FUNCTION_NAME(%PRE_CALL_ARGUMENTS %COMMA className == null ? null : className.data());\n"+
                          "}"}
        }
        ModifyFunction{
            signature: "setFont(QFont,const char*)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setPalette(QPalette,const char*)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "notify(QObject *, QEvent *)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
        }
        ModifyFunction{
            signature: "aboutQt()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "resolveInterface(const char *, int)const"
            access: Modification.Private
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "<QNativeInterface extends @Nullable QtObjectInterface> QNativeInterface"
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
        name: "QMouseEventTransition"
        until: 5
    }
    
    ObjectType{
        name: "QKeyEventTransition"
        until: 5
    }
    
    ObjectType{
        name: "QCommandLinkButton"
    }
    
    ObjectType{
        name: "QFileSystemModel"
        EnumType{
            name: "Roles"
            until: 5
        }
        EnumType{
            name: "Option"
            since: [5, 14]
            until: 5
        }
        ModifyFunction{
            signature: "setIconProvider(QFileIconProvider*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcIconProvider"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "iconProvider()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QGraphicsGridLayout"
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "addItem(QGraphicsLayoutItem*,int,int,Qt::Alignment)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "addItem(QGraphicsLayoutItem*,int,int,int,int,Qt::Alignment)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setAlignment(QGraphicsLayoutItem*,Qt::Alignment)"
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
            signature: "removeItem(QGraphicsLayoutItem *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QObject"
                location: Include.Global
            }
            Include{
                fileName: "QGraphicsWidget"
                location: Include.Global
            }
            Include{
                fileName: "QGraphicsObject"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QGraphicsGridLayout(QGraphicsLayoutItem*)"
            InjectCode{
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 instanceof QGraphicsWidget)\n"+
                              "QtJambi_LibraryUtilities.internal.setCppOwnership(%this);"}
            }
        }
        ModifyFunction{
            signature: "itemAt(int,int)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsLayout"
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "widgetEvent(QEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "addChildLayoutItem(QGraphicsLayoutItem*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QObject"
                location: Include.Global
            }
            Include{
                fileName: "QGraphicsWidget"
                location: Include.Global
            }
            Include{
                fileName: "QGraphicsObject"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QGraphicsLayout(QGraphicsLayoutItem*)"
            InjectCode{
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 instanceof QGraphicsWidget)\n"+
                              "QtJambi_LibraryUtilities.internal.setCppOwnership(%this);"}
            }
        }
        ModifyFunction{
            signature: "removeAt ( int )"
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "QGraphicsLayoutItem __item = itemAt(%1);"}
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(__item != null){\n"+
                              "QtJambi_LibraryUtilities.internal.setDefaultOwnership(__item);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "getContentsMargins(qreal*,qreal*,qreal*,qreal*)const"
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(%1.length<4) throw new IllegalArgumentException(\"An array of length 4 expected.\");"}
            }
            ModifyArgument{
                index: 1
                rename: "margins"
                ReplaceType{
                    modifiedType: "double @NonNull[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 4;\n"+
                                  "qreal* %out = qtjambi_array_cast<qreal*>(%env, %scope, %in, %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "qreal array[4];\n"+
                                  "if(%1)\n"+
                                  "    array[0] = *%1;\n"+
                                  "if(%2)\n"+
                                  "    array[1] = *%2;\n"+
                                  "if(%3)\n"+
                                  "    array[2] = *%3;\n"+
                                  "if(%4)\n"+
                                  "    array[3] = *%4;\n"+
                                  "%scope.addFinalAction([&](){\n"+
                                  "    if(%1)\n"+
                                  "        *%1 = array[0];\n"+
                                  "    if(%2)\n"+
                                  "        *%2 = array[1];\n"+
                                  "    if(%3)\n"+
                                  "        *%3 = array[2];\n"+
                                  "    if(%4)\n"+
                                  "        *%4 = array[3];\n"+
                                  "    });\n"+
                                  "jdoubleArray %out = qtjambi_array_cast<jdoubleArray>(%env, %scope, array, 4);"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal* %out = __qt_%1+1;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal* %out = __qt_%1+2;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal* %out = __qt_%1+3;"}
                }
            }
        }
        ModifyFunction{
            signature: "itemAt(int)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    InterfaceType{
        name: "QGraphicsLayoutItem"
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "setGraphicsItem(QGraphicsItem*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcItem"
                    action: ReferenceCount.Set
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QObject"
                location: Include.Global
            }
            Include{
                fileName: "QGraphicsWidget"
                location: Include.Global
            }
            Include{
                fileName: "QGraphicsObject"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QGraphicsLayoutItem(QGraphicsLayoutItem*)"
            InjectCode{
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 instanceof QGraphicsWidget)\n"+
                              "    QtJambi_LibraryUtilities.internal.setCppOwnership(%this);"}
            }
            until: [5, 14]
        }
        ModifyFunction{
            signature: "QGraphicsLayoutItem(QGraphicsLayoutItem*,bool)"
            InjectCode{
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 instanceof QGraphicsWidget)\n"+
                              "    QtJambi_LibraryUtilities.internal.setCppOwnership(%this);"}
            }
            since: [5, 15]
        }
        ModifyFunction{
            signature: "setParentLayoutItem(QGraphicsLayoutItem*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 instanceof QGraphicsWidget)\n"+
                              "    QtJambi_LibraryUtilities.internal.setCppOwnership(%this);\n"+
                              "else\n"+
                              "    QtJambi_LibraryUtilities.internal.setDefaultOwnership(%this);"}
            }
        }
        ModifyFunction{
            signature: "getContentsMargins(qreal*,qreal*,qreal*,qreal*)const"
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(%1.length<4) throw new IllegalArgumentException(\"An array of length 4 expected.\");"}
            }
            ModifyArgument{
                index: 1
                rename: "margins"
                ReplaceType{
                    modifiedType: "double @NonNull[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 4;\n"+
                                  "qreal* %out = qtjambi_array_cast<qreal*>(%env, %scope, %in, %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "qreal array[4];\n"+
                                  "if(%1)\n"+
                                  "    array[0] = *%1;\n"+
                                  "if(%2)\n"+
                                  "    array[1] = *%2;\n"+
                                  "if(%3)\n"+
                                  "    array[2] = *%3;\n"+
                                  "if(%4)\n"+
                                  "    array[3] = *%4;\n"+
                                  "DoublePointerArray pointerArray(%env, array, 4);\n"+
                                  "jdoubleArray %out = pointerArray.array();"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal* %out = __qt_%1+1;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal* %out = __qt_%1+2;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal* %out = __qt_%1+3;"}
                }
            }
            InjectCode{
                target: CodeClass.Shell
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                ArgumentMap{
                    index: 3
                    metaName: "%3"
                }
                ArgumentMap{
                    index: 4
                    metaName: "%4"
                }
                Text{content: "if(%1)\n"+
                              "    *%1 = array[0];\n"+
                              "if(%2)\n"+
                              "    *%2 = array[1];\n"+
                              "if(%3)\n"+
                              "    *%3 = array[2];\n"+
                              "if(%4)\n"+
                              "    *%4 = array[3];"}
            }
        }
        ModifyFunction{
            signature: "setOwnedByLayout(bool)"
            InjectCode{
                position: Position.Beginning
                Text{content: "boolean oldValue = this.ownedByLayout();"}
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(oldValue!=%1) {\n"+
                              "    if(%1) {\n"+
                              "        if(this.parentLayoutItem()!=null && QtJambi_LibraryUtilities.internal.isJavaOwnership(this)) {\n"+
                              "            QtJambi_LibraryUtilities.internal.setCppOwnership(this);\n"+
                              "        }\n"+
                              "    }else {\n"+
                              "        QtJambi_LibraryUtilities.internal.setDefaultOwnership(this);\n"+
                              "    }\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "graphicsItem()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "parentLayoutItem()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        InjectCode{
            target: CodeClass.JavaInterface
            Text{content: "/**\n"+
                          " * @see #getContentsMargins(double[])\n"+
                          " */\n"+
                          "@QtDeclaredFinal\n"+
                          "@QtUninvokable\n"+
                          "public default io.qt.core.QMarginsF contentsMargins(){\n"+
                          "    double[] mrg = {0.0, 0.0, 0.0, 0.0};\n"+
                          "    getContentsMargins(mrg);\n"+
                          "    return new io.qt.core.QMarginsF(mrg[0], mrg[1], mrg[2], mrg[3]);\n"+
                          "}"}
        }
    }
    
    ObjectType{
        name: "QGraphicsLinearLayout"
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "addItem(QGraphicsLayoutItem *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertItem(int, QGraphicsLayoutItem *)"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "removeItem(QGraphicsLayoutItem *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "setAlignment(QGraphicsLayoutItem*,Qt::Alignment)"
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
            signature: "setStretchFactor(QGraphicsLayoutItem*,int)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QObject"
                location: Include.Global
            }
            Include{
                fileName: "QGraphicsWidget"
                location: Include.Global
            }
            Include{
                fileName: "QGraphicsObject"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QGraphicsLinearLayout(QGraphicsLayoutItem*)"
            InjectCode{
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 instanceof QGraphicsWidget)\n"+
                              "    QtJambi_LibraryUtilities.internal.setCppOwnership(%this);"}
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsAnchorLayout"
        threadAffinity: "getPointerOwner(%1)"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "addAnchor(QGraphicsLayoutItem *, Qt::AnchorPoint, QGraphicsLayoutItem *, Qt::AnchorPoint)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            ModifyArgument{
                index: 3
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "addAnchors(QGraphicsLayoutItem *, QGraphicsLayoutItem *, Qt::Orientations)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            ModifyArgument{
                index: 2
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "addCornerAnchors(QGraphicsLayoutItem*,Qt::Corner,QGraphicsLayoutItem*,Qt::Corner)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            ModifyArgument{
                index: 3
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QObject"
                location: Include.Global
            }
            Include{
                fileName: "QGraphicsWidget"
                location: Include.Global
            }
            Include{
                fileName: "QGraphicsObject"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QGraphicsAnchorLayout(QGraphicsLayoutItem*)"
            InjectCode{
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 instanceof QGraphicsWidget)\n"+
                              "    QtJambi_LibraryUtilities.internal.setCppOwnership(%this);"}
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsProxyWidget"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "type() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "newProxyWidget(const QWidget*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
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
            signature: "createProxyForChildWidget(QWidget*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
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
            signature: "setWidget(QWidget*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null)\n"+
                              "    QtJambi_LibraryUtilities.internal.setCppOwnership(%1);\n"+
                              "else if (widget() != null)\n"+
                              "    QtJambi_LibraryUtilities.internal.setDefaultOwnership(widget());"}
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsWidget"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "children()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setPalette(QPalette)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setLayoutDirection(Qt::LayoutDirection)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "unsetLayoutDirection()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setFont(QFont)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "close()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setTabOrder(QGraphicsWidget *, QGraphicsWidget *)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "grabShortcut(QKeySequence, Qt::ShortcutContext)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "releaseShortcut(int)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setShortcutEnabled(int, bool)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setShortcutAutoRepeat(int, bool)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setFocusPolicy(Qt::FocusPolicy)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setStyle(QStyle*)"
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
            signature: "setMinimumSize(QSizeF)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setMaximumSize(QSizeF)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setMinimumSize(qreal,qreal)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setMaximumSize(qreal,qreal)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setMinimumWidth(qreal)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setMaximumWidth(qreal)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setMinimumHeight(qreal)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setMaximumHeight(qreal)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setContentsMargins(qreal,qreal,qreal,qreal)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setContentsMargins(QMarginsF)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setSizePolicy(QSizePolicy)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setSizePolicy(QSizePolicy::Policy,QSizePolicy::Policy,QSizePolicy::ControlType)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setAttribute(Qt::WidgetAttribute, bool)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "resize(qreal,qreal)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "resize(QSizeF)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setGeometry(qreal,qreal,qreal,qreal)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setGeometry(QRectF)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "adjustSize()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setWindowFlags(Qt::WindowFlags)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "addAction(QAction*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcActions"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "addActions(const QList<QAction *> &)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcActions"
                    action: ReferenceCount.AddAll
                }
            }
        }
        ModifyFunction{
            signature: "insertAction(QAction*,QAction*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            ModifyArgument{
                index: 2
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcActions"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "insertActions(QAction*,QList<QAction*>)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcActions"
                    action: ReferenceCount.AddAll
                }
            }
        }
        ModifyFunction{
            signature: "removeAction(QAction*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcActions"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "changeEvent(QEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "closeEvent(QCloseEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "grabKeyboardEvent(QEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "grabMouseEvent(QEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "hideEvent(QHideEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "moveEvent(QGraphicsSceneMoveEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "paintWindowFrame(QPainter*,const QStyleOptionGraphicsItem*,QWidget*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "resizeEvent(QGraphicsSceneResizeEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "showEvent(QShowEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "ungrabKeyboardEvent(QEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "ungrabMouseEvent(QEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "windowFrameEvent(QEvent*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "setStyle(QStyle*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcStyle"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setTabOrder(QGraphicsWidget*,QGraphicsWidget*)"
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
            signature: "setLayout(QGraphicsLayout*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(layout!=null && layout.parentLayoutItem()!=null && layout.parentLayoutItem()!=this) {\n"+
                              "    throw new IllegalArgumentException(\"Layout is already in use.\");\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "getContentsMargins(qreal*,qreal*,qreal*,qreal*)const"
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(%1.length<4) throw new IllegalArgumentException(\"An array of length 4 expected.\");"}
            }
            ModifyArgument{
                index: 1
                rename: "margins"
                ReplaceType{
                    modifiedType: "double @NonNull[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 4;\n"+
                                  "qreal* %out = qtjambi_array_cast<qreal*>(%env, %scope, %in, %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "qreal array[4];\n"+
                                  "if(%1)\n"+
                                  "    array[0] = *%1;\n"+
                                  "if(%2)\n"+
                                  "    array[1] = *%2;\n"+
                                  "if(%3)\n"+
                                  "    array[2] = *%3;\n"+
                                  "if(%4)\n"+
                                  "    array[3] = *%4;\n"+
                                  "DoublePointerArray pointerArray(%env, array, 4);\n"+
                                  "jdoubleArray %out = pointerArray.array();"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal* %out = __qt_%1+1;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal* %out = __qt_%1+2;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal* %out = __qt_%1+3;"}
                }
            }
            InjectCode{
                target: CodeClass.Shell
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                ArgumentMap{
                    index: 3
                    metaName: "%3"
                }
                ArgumentMap{
                    index: 4
                    metaName: "%4"
                }
                Text{content: "if(%1)\n"+
                              "    *%1 = array[0];\n"+
                              "if(%2)\n"+
                              "    *%2 = array[1];\n"+
                              "if(%3)\n"+
                              "    *%3 = array[2];\n"+
                              "if(%4)\n"+
                              "    *%4 = array[3];"}
            }
        }
        ModifyFunction{
            signature: "getWindowFrameMargins(qreal*,qreal*,qreal*,qreal*)const"
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(%1.length<4) throw new IllegalArgumentException(\"An array of length 4 expected.\");"}
            }
            ModifyArgument{
                index: 1
                rename: "margins"
                ReplaceType{
                    modifiedType: "double @NonNull[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 4;\n"+
                                  "qreal* %out = qtjambi_array_cast<qreal*>(%env, %scope, %in, %out_size);"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal* %out = __qt_%1+1;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal* %out = __qt_%1+2;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal* %out = __qt_%1+3;"}
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWidgets.java"
                quoteAfterLine: "class QGraphicsWidget___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "setLayout(QGraphicsLayout*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "graphicsItem()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "layout()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "parentLayoutItem()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QPlainTextDocumentLayout"
    }
    
    ObjectType{
        name: "QPlainTextEdit"
        ModifyFunction{
            signature: "setReadOnly(bool)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "createMimeDataFromSelection()const"
            threadAffinity: Affinity.UI
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
            signature: "createStandardContextMenu()"
            threadAffinity: Affinity.UI
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
            signature: "createStandardContextMenu(QPoint)"
            threadAffinity: Affinity.UI
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
            signature: "setDocument(QTextDocument*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcDocument"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "insertFromMimeData(const QMimeData*)"
            threadAffinity: Affinity.UI
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QStyledItemDelegate"
        ModifyFunction{
            signature: "createEditor(QWidget *,QStyleOptionViewItem,QModelIndex)const"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
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
            signature: "initStyleOption(QStyleOptionViewItem*,QModelIndex)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "setItemEditorFactory(QItemEditorFactory*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcItemEditorFactory"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setEditorData(QWidget*,QModelIndex)const"
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
            signature: "setModelData(QWidget*,QAbstractItemModel*,QModelIndex)const"
            threadAffinity: true
            ModifyArgument{
                index: 2
                threadAffinity: true
            }
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    
    ObjectType{
        name: "QPictureFormatPlugin"
        ModifyFunction{
            signature: "loadPicture(const QString &,const QString &,QPicture*)"
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
                NoNullPointer{
                }
            }
        }
    }
    
    ObjectType{
        name: "QStylePlugin"
        ModifyFunction{
            signature: "create(QString)"
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
        InjectCode{
            target: CodeClass.MetaInfo
            position: Position.Position1
            Text{content: "registerInterfaceID(typeId, QStyleFactoryInterface_iid);"}
        }
    }
    
    ObjectType{
        name: "QGesture"
    }
    
    ObjectType{
        name: "QGraphicsAnchor"
    }
    
    ObjectType{
        name: "QGraphicsEffect"
        ModifyFunction{
            signature: "draw(QPainter*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsObject"
        isValueOwner: true
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "children()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "show()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "hide()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "scroll(qreal,qreal,QRectF)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "update(QRectF)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "update(qreal,qreal,qreal,qreal)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setVisible(bool)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setToolTip(QString)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setFocus(Qt::FocusReason)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setToolTip(QString)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "grabMouse()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "grabKeyboard()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "parentItem() const"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "hasFocus() const"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "setCursor(QCursor)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setEnabled(bool)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setFocusProxy(QGraphicsItem*)"
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
            signature: "QGraphicsObject(QGraphicsItem*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 != null && %1.toGraphicsObject() == null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        ModifyFunction{
            signature: "setGroup(QGraphicsItemGroup*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setParentItem(QGraphicsItem*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "grabGesture(Qt::GestureType, Qt::GestureFlags)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "new io.qt.core.Qt.GestureFlag[0]"
                }
            }
        }
    }
    
    
    
    ObjectType{
        name: "QGraphicsRotation"
        ModifyFunction{
            signature: "applyTo(QMatrix4x4 *)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsScale"
        ModifyFunction{
            signature: "applyTo(QMatrix4x4 *)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QGraphicsTransform"
        ModifyFunction{
            signature: "applyTo(QMatrix4x4 *)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QPanGesture"
    }
    
    ObjectType{
        name: "QPinchGesture"
    }
    
    ObjectType{
        name: "QProxyStyle"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "setBaseStyle(QStyle*)"
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
        name: "QSwipeGesture"
    }
    
    ObjectType{
        name: "QTapAndHoldGesture"
    }
    
    ObjectType{
        name: "QTapGesture"
    }
    
    ObjectType{
        name: "QGraphicsOpacityEffect"
    }
    
    ObjectType{
        name: "QGraphicsColorizeEffect"
    }
    
    ObjectType{
        name: "QGraphicsBlurEffect"
    }
    
    ObjectType{
        name: "QGraphicsDropShadowEffect"
    }
    
    ObjectType{
        name: "QGestureRecognizer"
        ModifyFunction{
            signature: "registerRecognizer(QGestureRecognizer *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "create(QObject*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
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
            signature: "recognize(QGesture*,QObject*,QEvent*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
            ModifyArgument{
                index: 2
                threadAffinity: true
            }
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
        }
    }
    
    
    
    Rejection{
        className: "QS60Style"
    }
    
    ObjectType{
        name: "QTreeWidgetItemIterator"
        implementing: "Iterable<io.qt.widgets.QTreeWidgetItem>, java.util.Iterator<io.qt.widgets.QTreeWidgetItem>"
        threadAffinity: "getPointerOwner(%1)"

        EnumType{
            name: "IteratorFlag"
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "QTreeWidgetItemIterator(QTreeWidgetItemIterator)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator++(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator--(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator+=(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator-=(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator--()"
            remove: RemoveFlag.All
        }
        InjectCode{
            InsertTemplate{
                name: "core.self_iterator"
                Replace{
                    from: "%ELEMENT_TYPE"
                    to: "io.qt.widgets.QTreeWidgetItem"
                }
            }
        }
        ModifyFunction{
            signature: "QTreeWidgetItemIterator(QTreeWidget *,QTreeWidgetItemIterator::IteratorFlags)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "QtJambi_LibraryUtilities.internal.registerDependentObject(this, %1);"}
            }
        }
        ModifyFunction{
            signature: "QTreeWidgetItemIterator(QTreeWidgetItem *,QTreeWidgetItemIterator::IteratorFlags)"
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (%1 == null)\n"+
                              "    throw new NullPointerException(\"Argument '%1': null not expected.\");\n"+
                              "else if(%1.treeWidget()==null)\n"+
                              "    throw new IllegalArgumentException(\"Item does not belong to a tree widget.\");"}
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "QtJambi_LibraryUtilities.internal.registerDependentObject(this, %1.treeWidget());"}
            }
        }
        ModifyFunction{
            signature: "operator++()"
            rename: "next"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "QTreeWidgetItem * current = *(*__qt_this);"}
            }
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "io.qt.widgets.@Nullable QTreeWidgetItem"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(%in)\n"+
                                  "%out = qtjambi_cast<jobject>(%env, current);"}
                }
            }
        }
        ModifyFunction{
            signature: "operator*()const"
            rename: "hasNext"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "boolean"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = %in!=nullptr;"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QScroller"
        ModifyFunction{
            signature: "scroller(const QObject *)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QAccessibleWidget"
    }
    
    EnumType{
        name: "QScroller::Input"
    }
    
    EnumType{
        name: "QScroller::ScrollerGestureType"
    }
    
    EnumType{
        name: "QScroller::State"
    }
    
    ValueType{
        name: "QScrollerProperties"
    }
    
    EnumType{
        name: "QScrollerProperties::FrameRates"
    }
    
    EnumType{
        name: "QScrollerProperties::OvershootPolicy"
    }
    
    EnumType{
        name: "QScrollerProperties::ScrollMetric"
    }
    
    EnumType{
        name: "QOpenGLWidget::UpdateBehavior"
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLWidget"
        ModifyFunction{
            signature: "paintGL()"
            isPaintMethod: true
        }
        until: 5
    }
    
    NamespaceType{
        name: "QDrawBorderPixmap"
        javaName: "QtDrawUtil"

        EnumType{
            name: "DrawingHint"
        }
    }
    
    GlobalFunction{
        signature: "qDrawBorderPixmap(QPainter*, QRect, QMargins, QPixmap)"
        targetType: "QtDrawUtil"
        ModifyArgument{
            index: 1
            NoNullPointer{
            }
        }
    }
    
    GlobalFunction{
        signature: "qDrawBorderPixmap(QPainter*, QRect, QMargins, QPixmap, QRect, QMargins, QTileRules, QDrawBorderPixmap::DrawingHints)"
        targetType: "QtDrawUtil"
        ModifyArgument{
            index: 1
            NoNullPointer{
            }
        }
    }
    
    GlobalFunction{
        signature: "qDrawPlainRect(QPainter*, QRect, QColor, int, const QBrush*)"
        targetType: "QtDrawUtil"
        ModifyArgument{
            index: 1
            NoNullPointer{
            }
        }
        ModifyArgument{
            index: 5
            valueAsPointer: true
        }
    }
    
    GlobalFunction{
        signature: "qDrawPlainRect(QPainter*, int, int, int, int, QColor, int, const QBrush*)"
        targetType: "QtDrawUtil"
        ModifyArgument{
            index: 1
            NoNullPointer{
            }
        }
        ModifyArgument{
            index: 8
            valueAsPointer: true
        }
    }
    
    GlobalFunction{
        signature: "qDrawShadeLine(QPainter*, QPoint, QPoint, QPalette, bool, int, int)"
        targetType: "QtDrawUtil"
        ModifyArgument{
            index: 1
            NoNullPointer{
            }
        }
    }
    
    GlobalFunction{
        signature: "qDrawShadeLine(QPainter*, int, int, int, int, QPalette, bool, int, int)"
        targetType: "QtDrawUtil"
        ModifyArgument{
            index: 1
            NoNullPointer{
            }
        }
    }
    
    GlobalFunction{
        signature: "qDrawShadePanel(QPainter*, QRect, QPalette, bool, int, const QBrush*)"
        targetType: "QtDrawUtil"
        ModifyArgument{
            index: 1
            NoNullPointer{
            }
        }
        ModifyArgument{
            index: 6
            valueAsPointer: true
        }
    }
    
    GlobalFunction{
        signature: "qDrawShadePanel(QPainter*, int, int, int, int, QPalette, bool, int, const QBrush*)"
        targetType: "QtDrawUtil"
        ModifyArgument{
            index: 1
            NoNullPointer{
            }
        }
        ModifyArgument{
            index: 9
            valueAsPointer: true
        }
    }
    
    GlobalFunction{
        signature: "qDrawShadeRect(QPainter*, QRect, QPalette, bool, int, int, const QBrush*)"
        targetType: "QtDrawUtil"
        ModifyArgument{
            index: 1
            NoNullPointer{
            }
        }
        ModifyArgument{
            index: 7
            valueAsPointer: true
        }
    }
    
    GlobalFunction{
        signature: "qDrawShadeRect(QPainter*, int, int, int, int, QPalette, bool, int, int, const QBrush*)"
        targetType: "QtDrawUtil"
        ModifyArgument{
            index: 1
            NoNullPointer{
            }
        }
        ModifyArgument{
            index: 10
            valueAsPointer: true
        }
    }
    
    GlobalFunction{
        signature: "qDrawWinButton(QPainter*, QRect, QPalette, bool, const QBrush*)"
        targetType: "QtDrawUtil"
        ModifyArgument{
            index: 1
            NoNullPointer{
            }
        }
        ModifyArgument{
            index: 5
            valueAsPointer: true
        }
    }
    
    GlobalFunction{
        signature: "qDrawWinButton(QPainter*, int, int, int, int, QPalette, bool, const QBrush*)"
        targetType: "QtDrawUtil"
        ModifyArgument{
            index: 1
            NoNullPointer{
            }
        }
        ModifyArgument{
            index: 8
            valueAsPointer: true
        }
    }
    
    GlobalFunction{
        signature: "qDrawWinPanel(QPainter*, QRect, QPalette, bool, const QBrush*)"
        targetType: "QtDrawUtil"
        ModifyArgument{
            index: 1
            NoNullPointer{
            }
        }
        ModifyArgument{
            index: 5
            valueAsPointer: true
        }
    }
    
    GlobalFunction{
        signature: "qDrawWinPanel(QPainter*, int, int, int, int, QPalette, bool, const QBrush*)"
        targetType: "QtDrawUtil"
        ModifyArgument{
            index: 1
            NoNullPointer{
            }
        }
        ModifyArgument{
            index: 8
            valueAsPointer: true
        }
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'eventFilter(QObject * receiver, QEvent * event)' in 'QPanGesture'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'event(QEvent * event)' in 'QPanGesture'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'eventFilter(QObject * receiver, QEvent * event)' in 'QSwipeGesture'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'eventFilter(QObject * receiver, QEvent * event)' in 'QPinchGesture'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'event(QEvent * event)' in 'QPinchGesture'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type '*Private&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type '*Private\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type '*Private const\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QFontEngine\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QInputDialog::Type'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QAbstractUndoItem\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: non-public function '*' in interface '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: visibility of function '*' modified in class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: hiding of function '*' in class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unsupported default value 'QVector<FormatRange>()' of argument in function '*', class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unsupported default value 'QVariantList()' of argument in function '*', class '*'"}
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: protected function '*' in final class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QFileDialog::QFileDialog', unmatched parameter type 'const QFileDialogArgs&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QWidget::windowSurface', unmatched return type 'QWindowSurface*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QWidget::setWindowSurface', unmatched parameter type 'QWindowSurface*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: enum 'QStyleOption::StyleOptionType' does not have a type entry or is not an enum"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unhandled enum value: ~FlagMask in QMessageBox::StandardButton"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unmatched enum ~FlagMask"}
    SuppressedWarning{text: "WARNING(MetaInfoGenerator) :: class 'QGraphicsSceneEvent' inherits from polymorphic class 'QEvent', but has no polymorphic id set"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: Either add or remove specified for reference count variable '__rcMenus' in 'QMenu' but not both."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: Either add or remove specified for reference count variable '__rcMenus' in 'QMenuBar' but not both."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Rejected enum has no alternative...: QPalette::NColorRoles"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: namespace 'QtSharedPointer' does not have a type entry"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Object type 'QGraphicsEffect' passed as value. Resulting code will not compile."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Object type 'QGraphicsObject' passed as value. Resulting code will not compile."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: template baseclass 'QGenericMatrix*' of * is not known"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched * type '*QGenericMatrix*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched * type '*QPlatformMenu*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched * type '*QPlatformMenuBar*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping field 'QStyleOptionGroupBox::features' with unmatched type 'QStyleOptionFrameV2::FrameFeatures'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'children()const' for function modification in 'QGraphicsWidget' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'addAction(QAction*)' for function modification in 'QToolBar' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'update()' for function modification in 'QAbstractItemView' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'addAction(QAction*)' for function modification in 'QMenuBar' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'children()const' for function modification in 'QGraphicsObject' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'addAction(QAction*)' for function modification in 'QMenu' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'setStyleObject(QObject*)' for function modification in 'QStyleOption' not found. Possible candidates:"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'accept()' for function modification in 'QGestureEvent' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'ignore()' for function modification in 'QGestureEvent' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'isAccepted()const' for function modification in 'QGestureEvent' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'setAccepted(bool)' for function modification in 'QGestureEvent' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'setAsDockMenu()' for function modification in 'QMenu' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QSizePolicy::QSizePolicy', unmatched parameter type 'Bits'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QTableWidgetItem::tableModel', unmatched return type 'QTableModel*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QListWidgetItem::listModel', unmatched return type 'QListModel*'"}
    SuppressedWarning{text: "* private virtual function '*' in 'QHeaderView'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QColormap."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QTileRules."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QTableWidgetSelectionRange."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QStyleOption."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'hasEditFocus()const' for function modification in 'QWidget' not found. Possible candidates: "}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'setEditFocus(bool)' for function modification in 'QWidget' not found. Possible candidates: "}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QTreeWidgetItem."}
}
