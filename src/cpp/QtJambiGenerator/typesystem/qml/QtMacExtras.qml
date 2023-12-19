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
    packageName: "io.qt.macextras"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtMacExtras"
    module: "qtjambi.macextras"
    description: "Provides platform-specific APIs for macOS."
    defaultPPCondition: "defined(Q_OS_MACOS)"

    InjectCode{
        Text{content: "QtUtilities.initializePackage(\"io.qt.widgets\");"}
    }
    
    Rejection{
        className: "QCocoaWindowFunctions::BottomLeftClippedByNSWindowOffset"
    }
    
    NamespaceType{
        name: "QtMac"
    }
    
    ObjectType{
        name: "QMacPasteboardMime"
        EnumType{
            name: "QMacPasteboardMimeType"
        }
    }
    
    ObjectType{
        name: "QMacToolBar"
        ExtraIncludes{
            Include{
                fileName: "QtMacExtras/qmactoolbaritem.h"
                location: Include.Global
                ckeckAvailability: true
            }
        }
        ModifyFunction{
            signature: "nativeToolbar() const"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QMacToolBarItem"
        EnumType{
            name: "StandardItem"
        }
        ModifyFunction{
            signature: "nativeToolbarItem() const"
            remove: RemoveFlag.All
        }
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QHelpContentItem::QHelpContentItem', unmatched parameter type 'QHelpDBReader*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'changeEvent(QEvent * event)' in 'QHelpSearchQueryWidget'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'focusInEvent(QFocusEvent * focusEvent)' in 'QHelpSearchQueryWidget'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'changeEvent(QEvent * event)' in 'QHelpSearchResultWidget'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched *type '*NSImage*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched *type '*NSToolbar*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched *type '*NSData*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched *type '*CG*Ref*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'nativeToolbar()const' for function modification in 'QMacToolBar' not found. Possible candidates:*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'nativeToolbarItem()const' for function modification in 'QMacToolBarItem' not found. Possible candidates:*"}
}
