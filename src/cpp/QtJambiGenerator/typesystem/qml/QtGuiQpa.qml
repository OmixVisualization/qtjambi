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
    packageName: "io.qt.gui.qpa"
    defaultSuperClass: "QtObject"
    targetName: "QtJambiGuiQpa"
    module: "qtjambi"
    LoadTypeSystem{name: "QtGui"}
    RequiredLibrary{
        name: "QtGui"
    }

    ObjectType{
        name: "QPlatformIntegration"
        generate: "no-shell"
        ModifyFunction{
            signature: "QPlatformIntegration()"
            remove: RemoveFlag.All
        }
        EnumType{
            name: "Capability"
        }
        EnumType{
            name: "StyleHint"
        }

        Rejection{functionName: "call"}
        Rejection{functionName: "createPlatformPixmap"}
        Rejection{functionName: "createPlatformWindow"}
        Rejection{functionName: "createForeignWindow"}
        Rejection{functionName: "createPlatformBackingStore"}
        Rejection{functionName: "createPlatformOpenGLContext"}
        Rejection{functionName: "createPlatformSharedGraphicsCache"}
        Rejection{functionName: "createImagePaintEngine"}
        Rejection{functionName: "createEventDispatcher"}
        Rejection{functionName: "initialize"}
        Rejection{functionName: "destroy"}
        Rejection{functionName: "fontDatabase"}
        Rejection{functionName: "clipboard"}
        Rejection{functionName: "drag"}
        Rejection{functionName: "inputContext"}
        Rejection{functionName: "accessibility"}
        Rejection{functionName: "nativeInterface"}
        Rejection{functionName: "services"}
        Rejection{functionName: "createPlatformTheme"}
        Rejection{functionName: "createPlatformOffscreenSurface"}
        Rejection{functionName: "createPlatformSessionManager"}
        Rejection{functionName: "sync"}
        Rejection{functionName: "createPlatformVulkanInstance"}
        InjectCode{
            target: CodeClass.Java
            Text{content: "@QtUninvokable\n"+
                          "public native static QPlatformIntegration instance();"}
        }

        ModifyFunction{
            signature: "themeNames() const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QStringList"
                }
            }
        }
        ModifyFunction{
            signature: "possibleKeys(const QKeyEvent *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QList<java.lang.@QtPrimitiveType()@NonNull() Integer>"
                }
            }
        }
    }
}
