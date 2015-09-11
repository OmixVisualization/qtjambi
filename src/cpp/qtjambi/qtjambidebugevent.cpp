/****************************************************************************
**
** Copyright (C) 2011 Darryl L. Miles.  All rights reserved.
** Copyright (C) 2011 D L Miles Consulting Ltd.  All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
** 
** In addition, as a special exception, the copyright holders grant you
** certain additional rights. These rights are described in the Nokia Qt
** LGPL Exception version 1.0, included in the file LGPL_EXCEPTION.txt in
** this package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qtjambidebugevent_p.h"

#if defined(QTJAMBI_DEBUG_TOOLS)

#include <stdio.h>		// snprintf
#include <sys/types.h> 		// size_t

#include "qtjambi_global.h"
#include "qtjambi_core.h"

#if 0
#include <QtGui/QAccessibleEvent>
#include <QtGui/QActionEvent>
#include <QtCore/QChildEvent>
#endif

QString
QtJambiDebugEvent::qtjambi_event_to_string(QAccessibleEvent *e, const char *type_name)
{
#if 0
    // Move this to qtjambi_gui
    QString s = QString("%1 child=%2 value=%3")
        .arg(type_name)
        .arg((long)(void*)e->child(), 0, 16)
        .arg(e->value());
#else
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
#endif
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QActionEvent *e, const char *type_name)
{
#if 0
    // Move this to qtjambi_gui
    QString s = QString("%1 action=%2 before=%3")
        .arg(type_name)
        .arg((long)(void*)e->action(), 0, 16)
        .arg((long)(void*)e->before(), 0, 16);
#else
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
#endif
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QChildEvent *e, const char *type_name)
{
#if 0
    // Move this to qtjambi_core
    QString s = QString("%1 child=%2 added=%3 removed=%4 polished=%5")
        .arg(type_name)
        .arg((long)(void*)e->child(), 0, 16)
        .arg(e->added())
        .arg(e->removed())
        .arg(e->polished());
#else
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
#endif
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QCloseEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

#if 0
QString
QtJambiDebugEvent::qtjambi_event_to_string(QCustomEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}
#endif

QString
QtJambiDebugEvent::qtjambi_event_to_string(QDragLeaveEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QDropEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QDynamicPropertyChangeEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QFileOpenEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QFocusEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

#if QT_VERSION >= 0x040600
QString
QtJambiDebugEvent::qtjambi_event_to_string(QGestureEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}
#endif // QT_VERSION >= 0x040600

QString
QtJambiDebugEvent::qtjambi_event_to_string(QGraphicsSceneEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QHelpEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QHideEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QHoverEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QIconDragEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QInputEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QInputMethodEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QMoveEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QPaintEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QResizeEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QShortcutEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QShowEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QStatusTipEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QTimerEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QWhatsThisClickedEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QWindowStateChangeEvent *e, const char *type_name)
{
    Q_UNUSED(e);
    QString s = QString("%1")
        .arg(type_name);
    return s;
}

const char *
QtJambiDebugEvent::qtjambi_event_type_to_string(int type)
{
    switch(type) {
    case QEvent::None:					//   0 - invalid event
        return  "None";

    case QEvent::Timer:					//   1 - timer event
        return  "Timer";

    case QEvent::MouseButtonPress:			//   2 - mouse button pressed
        return  "MouseButtonPress";

    case QEvent::MouseButtonRelease:			//   3 - mouse button released
        return  "MouseButtonRelease";

    case QEvent::MouseButtonDblClick:			//   4 - mouse button double click
        return  "MouseButtonDblClick";

    case QEvent::MouseMove:				//   5 - mouse move
        return  "MouseMove";

    case QEvent::KeyPress:				//   6 - key pressed
        return  "KeyPress";

    case QEvent::KeyRelease:				//   7 - key released
        return  "KeyRelease";

    case QEvent::FocusIn:				//   8 - keyboard focus received
        return  "FocusIn";

    case QEvent::FocusOut:				//   9 - keyboard focus lost
        return  "FocusOut";

    case QEvent::Enter:					//  10 - mouse enters widget
        return  "Enter";

    case QEvent::Leave:					//  11 - mouse leaves widget
        return  "Leave";

    case QEvent::Paint:					//  12 - paint widget
        return  "Paint";

    case QEvent::Move:					//  13 - move widget
        return  "Move";

    case QEvent::Resize:				//  14 - resize widget
        return  "Resize";

    case QEvent::Create:				//  15 - after widget creation
        return  "Create";

    case QEvent::Destroy:				//  16 - during widget destruction
        return  "Destroy";

    case QEvent::Show:					//  17 - widget is shown
        return  "Show";

    case QEvent::Hide:					//  18 - widget is hidden
        return  "Hide";

    case QEvent::Close:					//  19 - request to close widget
        return  "Close";

    case QEvent::Quit:					//  20 - request to quit application
        return  "Quit";

    case QEvent::ParentChange:				//  21 - widget has been reparented
        return  "ParentChange";
#ifdef QT3_SUPPORT
    //Reparent = ParentChange,
#endif

    case QEvent::ThreadChange:				//  22 - object has changed thread
        return  "ThreadChange";

    case QEvent::WindowActivate:			//  24 - window was activate
        return  "WindowActivate";

    case QEvent::WindowDeactivate:			//  25 - window was deactivate
        return  "WindowDeactivate";

    case QEvent::ShowToParent:				//  26 - widget is shown to parent
        return  "ShowToParent";

    case QEvent::HideToParent:				//  27 - widget is hidden to parent
        return  "HideToParent";

#ifdef QT3_SUPPORT
    case QEvent::Accel:					//  30 - accelerator event
        return  "Accel";
#endif

    case QEvent::Wheel:					//  31 - wheel event
        return  "Wheel";

#ifdef QT3_SUPPORT
    case QEvent::AccelAvailable:			//  32 - accelerator available event
        return  "AccelAvailable";
    // AccelOverride = ShortcutOverride,      // accelerator override event
#endif

    case QEvent::WindowTitleChange:			//  33 - window title changed
        return  "WindowTitleChange";
#ifdef QT3_SUPPORT
    //CaptionChange = WindowTitleChange,
    //IconChange = WindowIconChange,
#endif

    case QEvent::WindowIconChange:			//  34 - icon changed
        return  "WindowIconChange";

    case QEvent::ApplicationWindowIconChange:		//  35 - application icon changed
        return  "ApplicationWindowIconChange";

    case QEvent::ApplicationFontChange:			//  36 - application font changed
        return  "ApplicationFontChange";

    case QEvent::ApplicationLayoutDirectionChange:	//  37 - application layout direction changed
        return  "ApplicationLayoutDirectionChange";

    case QEvent::ApplicationPaletteChange:		//  38 - application palette changed
        return  "ApplicationPaletteChange";

    case QEvent::PaletteChange:				//  39 - widget palette changed
        return  "PaletteChange";

    case QEvent::Clipboard:				//  40 - internal clipboard event
        return  "Clipboard";

    case QEvent::Speech:				//  42 - reserved for speech input
        return  "Speech";

    case QEvent::MetaCall:				//  43 - meta call event
        return  "MetaCall";

    case QEvent::SockAct:				//  50 - socket activation
        return  "SockAct";

    case QEvent::ShortcutOverride:			//  51 - shortcut override request
        return  "ShortcutOverride";

    case QEvent::DeferredDelete:			//  52 - deferred delete even
        return  "DeferredDelete";

    case QEvent::DragEnter:				//  60 - drag moves into widget
        return  "DragEnter";

    case QEvent::DragMove:				//  61 - drag moves in widget
        return  "DragMove";

    case QEvent::DragLeave:				//  62 - drag leaves or is cancelled
        return  "DragLeave";

    case QEvent::Drop:					//  63 - actual drop
        return  "Drop";

    case QEvent::DragResponse:				//  64 - drag accepted/rejected
        return  "DragResponse";

#ifdef QT3_SUPPORT
    case QEvent::ChildInsertedRequest:			//  67 - send ChildInserted compatibility events to receive
        return  "ChildInsertedRequest";
#endif

    case QEvent::ChildAdded:				//  68 - new child widget
        return  "ChildAdded";

    case QEvent::ChildPolished:				//  69 - polished child widget
        return  "ChildPolished";

#ifdef QT3_SUPPORT
    case QEvent::ChildInserted:				//  70 - compatibility child inserted
        return  "ChildInserted";
#endif

    case QEvent::ChildRemoved:				//  71 - deleted child widget
        return  "ChildRemoved";

#ifdef QT3_SUPPORT
    case QEvent::LayoutHint:				//  72 - compatibility relayout request
        return  "LayoutHint";
#endif

    case QEvent::ShowWindowRequest:			//  73 - widget's window should be mapped
        return  "ShowWindowRequest";

    case QEvent::PolishRequest:				//  74 - widget should be polished
        return  "PolishRequest";

    case QEvent::Polish:			//  75 - widget is polished
        return  "Polish";

    case QEvent::LayoutRequest:			//  76 - widget should be relayouted
        return  "LayoutRequest";

    case QEvent::UpdateRequest:			//  77 - widget should be repainted
        return  "UpdateRequest";

    case QEvent::UpdateLater:			//  78 - request update() later
        return  "UpdateLater";

    case QEvent::EmbeddingControl:		//  79 - ActiveX embedding
        return  "EmbeddingControl";

    case QEvent::ActivateControl:		//  80 - ActiveX activation
        return  "ActivateControl";

    case QEvent::DeactivateControl:		//  81 - ActiveX deactivation
        return  "DeactivateControl";

    case QEvent::ContextMenu:			//  82 - context popup menu
        return  "ContextMenu";

    case QEvent::InputMethod:			//  83 - input method
        return  "InputMethod";

    case QEvent::TabletMove:			//  87 - Wacom tablet event
        return  "TabletMove";

    case QEvent::LocaleChange:			//  88 - the system locale changed
        return  "LocaleChange";

    case QEvent::LanguageChange:		//  89 - the application language changed
        return  "LanguageChange";

    case QEvent::LayoutDirectionChange:		//  90 - the layout direction changed
        return  "LayoutDirectionChange";

    case QEvent::Style:				//  91 - internal style event
        return  "Style";

    case QEvent::TabletPress:			//  92 - tablet press
        return  "TabletPress";

    case QEvent::TabletRelease:			//  93 - tablet release
        return  "TabletRelease";

    case QEvent::OkRequest:			//  94 - CE (Ok) button pressed
        return  "OkRequest";

    case QEvent::HelpRequest:			//  95 - CE (?)  button pressed
        return  "HelpRequest";

    case QEvent::IconDrag:			//  96 - proxy icon dragged
        return  "IconDrag";

    case QEvent::FontChange:			//  97 - font has changed
        return  "FontChange";

    case QEvent::EnabledChange:			//  98 - enabled state has changed
        return  "EnabledChange";

    case QEvent::ActivationChange:		//  99 - window activation has changed
        return  "ActivationChange";

    case QEvent::StyleChange:			// 100 - style has changed
        return  "StyleChange";

    case QEvent::IconTextChange:		// 101 - icon text has changed
        return  "IconTextChange";

    case QEvent::ModifiedChange:		// 102 - modified state has changed
        return  "ModifiedChange";

    case QEvent::MouseTrackingChange:		// 109 - mouse tracking state has changed
        return  "MouseTrackingChange";

    case QEvent::WindowBlocked:			// 103 - window is about to be blocked modally
        return  "WindowBlocked";

    case QEvent::WindowUnblocked:		// 104 - windows modal blocking has ended
        return  "WindowUnblocked";

    case QEvent::WindowStateChange:		// 105
        return  "WindowStateChange";

    case QEvent::ToolTip:			// 110
        return  "ToolTip";

    case QEvent::WhatsThis:			// 111
        return  "WhatsThis";

    case QEvent::StatusTip:			// 112
        return  "StatusTip";

    case QEvent::ActionChanged:			// 113
        return  "ActionChanged";

    case QEvent::ActionAdded:			// 114
        return  "ActionAdded";

    case QEvent::ActionRemoved:			// 115
        return  "ActionRemoved";

    case QEvent::FileOpen:			// 116 - file open request
        return  "FileOpen";

    case QEvent::Shortcut:			// 117 - shortcut trigger
        return  "Shortcut";

    case QEvent::WhatsThisClicked:		// 118
        return  "WhatsThisClicked";


    case QEvent::ToolBarChange:				// 120 - toolbar visibility toggled
        return  "ToolBarChange";

    case QEvent::ApplicationActivate:			// 121 - application has been changed to active
        return  "ApplicationActivate";
    // ApplicationActivated = ApplicationActivate,// deprecated

    case QEvent::ApplicationDeactivate:			// 122 - application has been changed to inactive
        return  "ApplicationDeactivate";
    // ApplicationDeactivated = ApplicationDeactivate,// deprecated

    case QEvent::QueryWhatsThis:			// 123 - query what's this widget hel
        return  "QueryWhatsThis";

    case QEvent::EnterWhatsThisMode:			// 124
        return  "EnterWhatsThisMode";

    case QEvent::LeaveWhatsThisMode:			// 125
        return  "LeaveWhatsThisMode";

    case QEvent::ZOrderChange:				// 126 - child widget has had its z-order change
        return  "ZOrderChange";

    case QEvent::HoverEnter:				// 127 - mouse cursor enters a hover widget
        return  "HoverEnter";

    case QEvent::HoverLeave:				// 128 - mouse cursor leaves a hover widget
        return  "HoverLeave";

    case QEvent::HoverMove:				// 129 - mouse cursor move inside a hover widget
        return  "HoverMove";

    case QEvent::ParentAboutToChange:			// 131 - sent just before the parent change is done
        return  "ParentAboutToChange";

    case QEvent::WinEventAct:				// 132 - win event activation
        return  "WinEventAct";

    // last event id used = 132

#ifdef QT_KEYPAD_NAVIGATION
    case QEvent::EnterEditFocus:			// 150 - enter edit mode in keypad navigation
        return  "EnterEditFocus";

    case QEvent::LeaveEditFocus:			// 151 - enter edit mode in keypad navigation
        return  "LeaveEditFocus";
#endif
    case QEvent::AcceptDropsChange:			// 152
        return  "AcceptDropsChange";

    case QEvent::ZeroTimerEvent:			// 154 - Used for Windows Zero timer event
        return  "ZeroTimerEvent";

    case QEvent::GraphicsSceneMouseMove:		// 155 - GraphicsView
        return  "GraphicsSceneMouseMove";

    case QEvent::GraphicsSceneMousePress:		// 156
        return  "GraphicsSceneMousePress";

    case QEvent::GraphicsSceneMouseRelease:		// 157
        return  "GraphicsSceneMouseRelease";

    case QEvent::GraphicsSceneMouseDoubleClick:		// 158
        return  "GraphicsSceneMouseDoubleClick";

    case QEvent::GraphicsSceneContextMenu:		// 159
        return  "GraphicsSceneContextMenu";

    case QEvent::GraphicsSceneHoverEnter:		// 160
        return  "GraphicsSceneHoverEnter";

    case QEvent::GraphicsSceneHoverMove:		// 161
        return  "GraphicsSceneHoverMove";

    case QEvent::GraphicsSceneHoverLeave:		// 162
        return  "GraphicsSceneHoverLeave";

    case QEvent::GraphicsSceneHelp:			// 163
        return  "GraphicsSceneHelp";

    case QEvent::GraphicsSceneDragEnter:		// 164
        return  "GraphicsSceneDragEnter";

    case QEvent::GraphicsSceneDragMove:			// 165
        return  "GraphicsSceneDragMove";

    case QEvent::GraphicsSceneDragLeave:		// 166
        return  "GraphicsSceneDragLeave";

    case QEvent::GraphicsSceneDrop:			// 167
        return  "GraphicsSceneDrop";

    case QEvent::GraphicsSceneWheel:			// 168
        return  "GraphicsSceneWheel";

    case QEvent::KeyboardLayoutChange:			// 169 - keyboard layout changed
        return  "KeyboardLayoutChange";

    case QEvent::DynamicPropertyChange:			// 170 - A dynamic property was changed through setProperty/property
        return  "DynamicPropertyChange";

    case QEvent::TabletEnterProximity:			// 171
        return  "TabletEnterProximity";

    case QEvent::TabletLeaveProximity:			// 172
        return  "TabletLeaveProximity";

    case QEvent::NonClientAreaMouseMove:		// 173
        return  "NonClientAreaMouseMove";

    case QEvent::NonClientAreaMouseButtonPress:		// 174
        return  "NonClientAreaMouseButtonPress";

    case QEvent::NonClientAreaMouseButtonRelease:	// 175
        return  "NonClientAreaMouseButtonRelease";

    case QEvent::NonClientAreaMouseButtonDblClick:	// 176
        return  "NonClientAreaMouseButtonDblClick";

    case QEvent::MacSizeChange:				// 177 - when the Qt::WA_Mac{Normal,Small,Mini}Size changed
        return  "MacSizeChange";

    case QEvent::ContentsRectChange:			// 178 - sent by QWidget::setContentsMargins (internal)
        return  "ContentsRectChange";

    case QEvent::MacGLWindowChange:			// 179 - Internal! the window of the GLWidget has changed
        return  "MacGLWindowChange";

    case QEvent::FutureCallOut:		 		// 180
        return  "FutureCallOut";

    case QEvent::GraphicsSceneResize:			// 181
        return  "GraphicsSceneResize";

    case QEvent::GraphicsSceneMove:			// 182
        return  "GraphicsSceneMove";

    case QEvent::CursorChange:				// 183
        return  "CursorChange";

    case QEvent::ToolTipChange:				// 184
        return  "ToolTipChange";

    case QEvent::NetworkReplyUpdated:			// 185 - Internal for QNetworkReply
        return  "NetworkReplyUpdated";

    case QEvent::GrabMouse:				// 186
        return  "GrabMouse";

    case QEvent::UngrabMouse:				// 187
        return  "UngrabMouse";

    case QEvent::GrabKeyboard:				// 188
        return  "GrabKeyboard";

    case QEvent::UngrabKeyboard:			// 189
        return  "UngrabKeyboard";

    case QEvent::MacGLClearDrawable:			// 191 - Internal Cocoa, the window has changed, so we must clear
        return  "MacGLClearDrawable";

#if QT_VERSION >= 0x040600
    case QEvent::StateMachineSignal:			// 192
        return  "StateMachineSignal";

    case QEvent::StateMachineWrapped:			// 193
        return  "StateMachineWrapped";

    case QEvent::TouchBegin:				// 194
        return  "TouchBegin";

    case QEvent::TouchUpdate:				// 195
        return  "TouchUpdate";

    case QEvent::TouchEnd:				// 196
        return  "TouchEnd";

#ifndef QT_NO_GESTURES
    case QEvent::NativeGesture:				// 197 - Internal for platform gesture support
        return  "NativeGesture";
#endif
#ifndef QT_NO_GESTURES
    case QEvent::Gesture:				// 198
        return  "Gesture";
#endif

    case QEvent::RequestSoftwareInputPanel:		// 199
        return  "RequestSoftwareInputPanel";

    case QEvent::CloseSoftwareInputPanel:		// 200
        return  "CloseSoftwareInputPanel";

#ifndef QT_NO_GESTURES
    case QEvent::GestureOverride:			// 202
        return  "GestureOverride";
#endif

    case QEvent::WinIdChange:				// 203
        return  "WinIdChange";
#endif // QT_VERSION >= 0x040600

    case 512:						// 512 reserved for Qt Jambi's MetaCall event
        return "QtJambiMetaCall";
    case 513:						// 513 reserved for Qt Jambi's DeleteOnMainThread event
        return "QtJambiDeleteOnMainThread";

    case QEvent::User:		// 1000 - first user event id
        return  "User";

    case QEvent::MaxUser:	// 65535 - last user event id
        return  "MaxUser";
    }

/*
// no longer available in Qt5 <= 5.2
    case QEvent::AccessibilityPrepare:		//  86 - accessibility information is requested
        return  "AccessibilityPrepare";

    case QEvent::AccessibilityHelp:			// 119 - accessibility help text request
        return  "AccessibilityHelp";

    case QEvent::AccessibilityDescription:		// 130 - accessibility description text request
        return  "AccessibilityDescription";

    case QEvent::MenubarUpdated:			// 153 - Support event for Q3MainWindow, which needs to knwow when QMenubar is updated.
        return  "MenubarUpdated";

    case QEvent::UpdateSoftKeys:			// 201 - Internal for compressing soft key updated
        return  "UpdateSoftKeys";

*/
    return 0;
}

const char *
QtJambiDebugEvent::qtjambi_event_type_to_string(char *buf, size_t buflen, int type, bool with_id)
{
    const char *s = qtjambi_event_type_to_string(type);
    if(!s)
        s = "UNKNOWN";

    if(with_id) {
        snprintf(buf, buflen, "%s(%d)", s, type);
        return buf;
    }

    return s;
}

QString
QtJambiDebugEvent::qtjambi_event_to_string(QEvent *event)
{
    if(!event)
        return QString();

    int type = event->type();

    char tmpbuf[64];
    const char *typedesc = qtjambi_event_type_to_string(tmpbuf, sizeof(tmpbuf), type, true);

    switch(type) {
    case QEvent::None:				// 0
//    case QEvent::AccessibilityPrepare:		// 86
    default:
    {
        return QString(typedesc);
    }

//    case QEvent::AccessibilityHelp:		// 119
//    case QEvent::AccessibilityDescription:	// 130
//    {
//        QAccessibleEvent *e = static_cast<QAccessibleEvent *>(event);
//        return qtjambi_event_to_string(e, typedesc);
//    }

    case QEvent::ActionChanged:			// 113
    case QEvent::ActionAdded:			// 114
    case QEvent::ActionRemoved:			// 115

    case QEvent::ChildAdded:			// 68
    case QEvent::ChildPolished:			// 69
#ifdef QT3_SUPPORT
    case QEvent::ChildInserted:			// 70 Qt3Support only
#endif
    case QEvent::ChildRemoved:			// 71
    {
        QChildEvent *e = static_cast<QChildEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::Close:				// 19
    {
        QCloseEvent *e = static_cast<QCloseEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::ContextMenu:			// 82
    {
        QContextMenuEvent *e = static_cast<QContextMenuEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::DragEnter:			// 60  CHECKME
    case QEvent::DragMove:			// 61
    case QEvent::DragLeave:			// 62  CHECKME
    {
        QDragMoveEvent *e = static_cast<QDragMoveEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::Drop:				// 63
    {
        QDropEvent *e = static_cast<QDropEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::FileOpen:			// 116
    {
        QFileOpenEvent *e = static_cast<QFileOpenEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::FocusIn:			// 8
    case QEvent::FocusOut:			// 9
    {
        QFocusEvent *e = static_cast<QFocusEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::Hide:				// 18
    {
        QHideEvent *e = static_cast<QHideEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::HoverEnter:			// 127
    case QEvent::HoverLeave:			// 128
    case QEvent::HoverMove:			// 129
    {
        QHoverEvent *e = static_cast<QHoverEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::IconDrag:			// 96
    {
        QIconDragEvent *e = static_cast<QIconDragEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::InputMethod:			// 83
    {
        QInputMethodEvent *e = static_cast<QInputMethodEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::KeyPress:			//  6
    case QEvent::KeyRelease:			//  7
    case QEvent::ShortcutOverride:		// 51
    {
        QKeyEvent *e = static_cast<QKeyEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }
// GraphicsScene

    case QEvent::MouseButtonPress:		//  2
    case QEvent::MouseButtonRelease:		//  3
    case QEvent::MouseButtonDblClick:		//  4
    case QEvent::MouseMove:			//  5
    {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::Move:				// 13
    {
        QMoveEvent *e = static_cast<QMoveEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::Paint:				// 12
    {
        QPaintEvent *e = static_cast<QPaintEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::Resize:			// 14
    {
        QResizeEvent *e = static_cast<QResizeEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::Shortcut:			// 117
    {
        QShortcutEvent *e = static_cast<QShortcutEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::Show:				// 17
    {
        QShowEvent *e = static_cast<QShowEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

#if 0
    case QEvent::SockAct:			// 50
    {
    // FIXME ??
        QEvent *e = static_cast<QEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }
#endif

// CHECKME StateMachine
    case QEvent::StatusTip:			// 112
    {
        QStatusTipEvent *e = static_cast<QStatusTipEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::TabletMove:			// 87
    case QEvent::TabletPress:			// 92
    case QEvent::TabletRelease:			// 93
    case QEvent::TabletEnterProximity:		// 171
    case QEvent::TabletLeaveProximity:		// 172
    {
        QTabletEvent *e = static_cast<QTabletEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::Timer:				// 1
    {
        QTimerEvent *e = static_cast<QTimerEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::ToolTip:			// 110
    case QEvent::WhatsThis:			// 111
    case QEvent::GraphicsSceneHelp:		// 163
    {
        QHelpEvent *e = static_cast<QHelpEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::Wheel:				// 31
    {
        QWheelEvent *e = static_cast<QWheelEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

    case QEvent::WindowStateChange:		// 105
    {
        QWindowStateChangeEvent *e = static_cast<QWindowStateChangeEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

#if QT_VERSION >= 0x040600
    case QEvent::TouchBegin:			// 194
    case QEvent::TouchUpdate:			// 195
    case QEvent::TouchEnd:			// 196
    {
        QTouchEvent *e = static_cast<QTouchEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }

 #ifndef QT_NO_GESTURES
    case QEvent::Gesture:			// 198
    case QEvent::GestureOverride:		// 202
    {
        QGestureEvent *e = static_cast<QGestureEvent *>(event);
        return qtjambi_event_to_string(e, typedesc);
    }
 #endif
#endif // QT_VERSION >= 0x040600
    }

    return QString();
}

const char *
QtJambiDebugEvent::qtjambi_event_desc_to_string(char *buf, size_t buflen, QEvent *event)
{
    QString typedesc = qtjambi_event_to_string(event);
    if(typedesc.isNull())
        return 0;
    QByteArray ba = typedesc.toLocal8Bit(); // keeps constData() alive for strncpy()
    const char *s = ba.constData();
    if(s) {
        strncpy(buf, s, buflen);
        buf[buflen - 1] = '\0';
        return buf;
    }
    return 0;
}

#else // QTJAMBI_DEBUG_TOOLS
#  error "Don't include this file without QTJAMBI_DEBUG_TOOLS defined"
#endif
