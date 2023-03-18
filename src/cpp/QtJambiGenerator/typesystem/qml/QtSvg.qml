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
    packageName: "io.qt.svg"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtSvg"
    module: "qtjambi.svg"
    description: "Classes for displaying the contents of SVG files. Supports a subset of the SVG 1.2 Tiny standard."
    Rejection{
        className: "QGraphicsSvgItem"
        enumName: "enum_1"
        until: 5
    }
    
    NamespaceType{
        name: "QSvg"
    }

    EnumType{
        name: "QSvgGenerator::SvgVersion"
        since: [6,5]
    }
    
    EnumType{
        name: "QGraphicsSvgItem::enum_1"
        until: 5
    }
    
    ObjectType{
        name: "QSvgRenderer"
    }
    
    ObjectType{
        name: "QSvgWidget"
        ExtraIncludes{
            Include{
                fileName: "QByteArray"
                location: Include.Global
            }
            Include{
                fileName: "QSvgRenderer"
                location: Include.Global
            }
            Include{
                fileName: "QSize"
                location: Include.Global
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QSvgGenerator"
        ModifyFunction{
            signature: "setOutputDevice(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcOutputDevice"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "devType() const"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QGraphicsSvgItem"
        ExtraIncludes{
            Include{
                fileName: "QtJambiWidgets/hashes.h"
                location: Include.Global
            }
            Include{
                fileName: "QGraphicsItem"
                location: Include.Global
            }
            Include{
                fileName: "QSvgRenderer"
                location: Include.Global
            }
            Include{
                fileName: "QSize"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "type() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QGraphicsSvgItem(QGraphicsItem*)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
        }
        ModifyFunction{
            signature: "QGraphicsSvgItem(QString,QGraphicsItem*)"
            ModifyArgument{
                index: 2
                threadAffinity: true
            }
        }
        ModifyFunction{
            signature: "setSharedRenderer(QSvgRenderer*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            InjectCode{
                position: Position.Beginning
                Text{content: "QSvgRenderer oldRenderer = renderer();"}
            }
            InjectCode{
                position: Position.End
                Text{content: "if (oldRenderer != null)\n"+
                              "    QtJambi_LibraryUtilities.internal.setDefaultOwnership(oldRenderer);"}
            }
        }
        ModifyFunction{
            signature: "QGraphicsSvgItem(QGraphicsItem*)"
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
            signature: "QGraphicsSvgItem(const QString &,QGraphicsItem*)"
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if (%2 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
        until: 5
    }
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: protected function '*' in final class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: visibility of function '*' modified in class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: hiding of function '*' in class '*'"}
}
