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
    packageName: "io.qt.x11extras"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtX11Extras"
    module: "qtjambi.x11extras"
    description: "Provides platform-specific APIs for X11."
    defaultPPCondition: "defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)"

    RequiredLibrary{
        name: "QtGui"
        mode: "Mandatory"
    }

    ObjectType{
        name: "QX11Info"
        Rejection{className: "PeekerCallback"}

        EnumType{
            name: "PeekOption"
            since: [5, 10]
        }
    }
    
    Rejection{
        className: "QWaylandWindowFunctions::IsWindowSync"
    }
    
    Rejection{
        className: "QWaylandWindowFunctions::SetWindowDeSync"
    }
    
    Rejection{
        className: "QWaylandWindowFunctions::SetWindowSync"
    }
    
    Rejection{
        className: "QXcbIntegrationFunctions::XEmbedSystemTrayVisualHasAlphaChannel"
    }
    
    Rejection{
        className: "QXcbScreenFunctions::VirtualDesktopNumber"
    }
    
    Rejection{
        className: "QXcbWindowFunctions::RequestSystemTrayWindowDock"
    }
    
    Rejection{
        className: "QXcbWindowFunctions::SetParentRelativeBackPixmap"
    }
    
    Rejection{
        className: "QXcbWindowFunctions::SetWmWindowIconText"
    }
    
    Rejection{
        className: "QXcbWindowFunctions::SetWmWindowRole"
    }
    
    Rejection{
        className: "QXcbWindowFunctions::SetWmWindowType"
    }
    
    Rejection{
        className: "QXcbWindowFunctions::SystemTrayWindowGlobalGeometry"
    }
    
    Rejection{
        className: "QXcbWindowFunctions::VisualId"
    }
    
    Rejection{
        className: "QEglFSFunctions::LoadKeymapType"
    }
    
    Rejection{
        className: "QEglFSFunctions::Vsp2AddBlendListenerType"
    }
    
    Rejection{
        className: "QEglFSFunctions::Vsp2AddLayerType"
    }
    
    Rejection{
        className: "QEglFSFunctions::Vsp2RemoveLayerType"
    }
    
    Rejection{
        className: "QEglFSFunctions::Vsp2SetLayerAlphaType"
    }
    
    Rejection{
        className: "QEglFSFunctions::Vsp2SetLayerBufferType"
    }
    
    Rejection{
        className: "QEglFSFunctions::Vsp2SetLayerPositionType"
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched *type 'Display*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched *type 'xcb_connection_t*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched *type 'QX11Info::PeekerCallback'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature '*' for function modification in 'QXcbWindowFunctions' not found. Possible candidates:*"}
}
