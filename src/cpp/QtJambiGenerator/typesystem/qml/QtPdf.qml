/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtPdf"
    module: "qtjambi.pdf"
    description: "Classes and functions for rendering PDF documents."
    RequiredLibrary{
        name: "QtNetwork"
    }
    
    EnumType{
        name: "QPdfSearchModel::Role"
    }
    
    EnumType{
        name: "QPdfDocument::DocumentError"
        until: [6, 3]
    }
    
    EnumType{
        name: "QPdfDocument::Error"
        since: [6, 4]
    }
    
    EnumType{
        name: "QPdfDocument::PageModelRole"
        since: [6, 4]
    }
    
    EnumType{
        name: "QPdfBookmarkModel::StructureMode"
    }
    
    EnumType{
        name: "QPdfBookmarkModel::Role"
    }
    
    ValueType{
        name: "QPdfLink"
        ModifyFunction{
            signature: "operator=(QPdfLink)"
            remove: RemoveFlag.All
        }
        since: [6, 4]
    }
    
    ObjectType{
        name: "QPdfBookmarkModel"
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
    
    ValueType{
        name: "QPdfDestination"
        noMetaType: true
        generate: "no-shell"
        ModifyFunction{
            signature: "operator=(QPdfDestination)"
            remove: RemoveFlag.All
        }
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
    
    EnumType{
        name: "QPdfDocument::MetaDataField"
    }
    
    EnumType{
        name: "QPdfDocument::Status"
    }
    
    ObjectType{
        name: "QPdfDocument"
    }
    
    ValueType{
        name: "QPdfDocumentRenderOptions"
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
    
    EnumType{
        name: "QPdfPageRenderer::RenderMode"
    }
    
    ObjectType{
        name: "QPdfPageRenderer"
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
    
    ValueType{
        name: "QPdfSelection"
        noMetaType: true
        ExtraIncludes{
            Include{
                fileName: "QtJambiGui/hashes.h"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QPdfSelection(QPdfSelection)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(QPdfSelection)"
            remove: RemoveFlag.All
        }
    }
    
    NamespaceType{
        name: "QPdf"
        until: [6, 3]
    }
    
    EnumType{
        name: "QPdf::RenderFlag"
        flags: "QPdf::RenderFlags"
        until: [6, 3]
    }
    
    EnumType{
        name: "QPdf::Rotation"
        until: [6, 3]
    }
    
    EnumType{
        name: "QPdfDocumentRenderOptions::RenderFlag"
        flags: "QPdfDocumentRenderOptions::RenderFlags"
        since: [6, 4]
    }
    
    EnumType{
        name: "QPdfDocumentRenderOptions::Rotation"
        since: [6, 4]
    }
    
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: Value type 'QPdfDestination' is missing a default constructor.*"}
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: Value type 'QPdfSelection' is missing a default constructor.*"}
}
