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
    packageName: "io.qt.pdf"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtPdf"
    module: "qtjambi.pdf"
    description: "Classes and functions for rendering PDF documents."
    RequiredLibrary{
        name: "QtNetwork"
    }
    
    ValueType{
        name: "QPdfLink"
        since: [6, 4]
    }
    
    ObjectType{
        name: "QPdfBookmarkModel"

        EnumType{
            name: "StructureMode"
        }

        EnumType{
            name: "Role"
        }
        ModifyFunction{
            signature: "setDocument(QPdfDocument *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDocument"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QPdfPageNavigator"
        since: [6, 4]
    }
    
    ObjectType{
        name: "QPdfDestination"
        generate: "no-shell"
        ModifyFunction{
            signature: "QPdfDestination()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QPdfDestination(QPdfDestination)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QPdfDestination(int, QPointF, qreal)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QPdfDocument"

        EnumType{
            name: "DocumentError"
            until: [6, 3]
        }

        EnumType{
            name: "Error"
            since: [6, 4]
        }

        EnumType{
            name: "PageModelRole"
            since: [6, 4]
        }

        EnumType{
            name: "MetaDataField"
        }

        EnumType{
            name: "Status"
        }
    }
    
    ValueType{
        name: "QPdfDocumentRenderOptions"

        EnumType{
            name: "RenderFlag"
            since: [6, 4]
        }

        EnumType{
            name: "Rotation"
            since: [6, 4]
        }
    }
    
    ObjectType{
        name: "QPdfPageNavigation"
        ModifyFunction{
            signature: "setDocument(QPdfDocument *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDocument"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QPdfPageRenderer"
        EnumType{
            name: "RenderMode"
        }
        ModifyFunction{
            signature: "setDocument(QPdfDocument *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDocument"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QPdfSearchModel"

        EnumType{
            name: "Role"
        }
        ModifyFunction{
            signature: "setDocument(QPdfDocument *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDocument"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ValueType{
        name: "QPdfSearchResult"
        CustomConstructor{
            type: CustomConstructor.Copy
            Text{content: "new(placement) QPdfSearchResult(*copy);"}
        }
        CustomConstructor{
            type: CustomConstructor.Move
            Text{content: "new(placement) QPdfSearchResult(std::move(*copy));"}
        }
    }
    
    ObjectType{
        name: "QPdfSelection"
        ExtraIncludes{
            Include{
                fileName: "QtJambiGui/hashes.h"
                location: Include.Global
            }
        }
    }
    
    NamespaceType{
        name: "QPdf"

        EnumType{
            name: "RenderFlag"
            until: [6, 3]
        }

        EnumType{
            name: "Rotation"
            until: [6, 3]
        }
        until: [6, 3]
    }

    ObjectType{
        name: "QPdfLinkModel"
        EnumType{
            name: "Role"
        }
        ModifyFunction{
            signature: "setDocument(QPdfDocument *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDocument"
                    action: ReferenceCount.Set
                }
            }
        }
        since: [6, 6]
    }
    
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: Value type 'QPdfDestination' is missing a default constructor.*"}
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: Value type 'QPdfSelection' is missing a default constructor.*"}
}
