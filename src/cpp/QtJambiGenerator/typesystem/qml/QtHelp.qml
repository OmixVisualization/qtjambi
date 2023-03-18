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
    packageName: "io.qt.help"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtHelp"
    module: "qtjambi.help"
    description: "Classes for integrating documentation into applications, similar to Qt Assistant."

    RequiredLibrary{
        name: "QtNetwork"
        mode: RequiredLibrary.ProvideOnly
        platforms: ["linux", "macos"]
    }
    
    ObjectType{
        name: "QHelpContentItem"
        ModifyFunction{
            signature: "child(int)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "parent()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    
    ObjectType{
        name: "QHelpContentWidget"
    }
    
    ObjectType{
        name: "QHelpIndexWidget"
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{
                until: 5
                content: String.raw`inline hash_type qHash(const QMap<QString, QUrl> &value, hash_type seed = 0){
    QtPrivate::QHashCombineCommutative hash;
    seed = hash(seed, value.size());
    for(const QString& key : value.keys()){
        seed = hash(seed, key);
        seed = hash(seed, value.value(key));
    }
    return seed;
}`}
            Text{
                since: 6
                content: String.raw`inline hash_type qHash(const QMultiMap<QString, QUrl> &value, hash_type seed = 0){
    QtPrivate::QHashCombineCommutative hash;
    seed = hash(seed, value.keys().size());
    for(const QString& key : value.keys()){
        seed = hash(seed, key);
        seed = hash(seed, value.values(key));
    }
    return seed;
}`}
        }
    }
    
    ObjectType{
        name: "QHelpIndexModel"
    }
    
    ObjectType{
        name: "QHelpContentModel"
        ModifyFunction{
            signature: "contentItemAt(QModelIndex)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QHelpEngineCore"
        ModifyFunction{
            signature: "QHelpEngineCore(const QString &, QObject *)"
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
    }
    
    ObjectType{
        name: "QHelpEngine"
        ModifyFunction{
            signature: "QHelpEngine(const QString &, QObject *)"
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
    }
    
    ObjectType{
        name: "QHelpSearchEngine"

        Rejection{
            className: "SearchHit"
        }
        ModifyFunction{
            signature: "QHelpSearchEngine ( QHelpEngineCore *, QObject *)"
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%1"
                }
                Text{content: "if (%1 != null) QtJambi_LibraryUtilities.internal.setCppOwnership(this);"}
            }
        }
    }
    
    ValueType{
        name: "QHelpSearchQuery"
        EnumType{
            name: "FieldName"
        }
    }
    
    ValueType{
        name: "QHelpSearchResult"
    }
    
    ObjectType{
        name: "QHelpSearchQueryWidget"
        ModifyFunction{
            signature: "QHelpSearchQueryWidget ( QWidget *)"
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
        name: "QHelpSearchResultWidget"
    }
    
    ObjectType{
        name: "QHelpFilterEngine"
        since: [5, 13]
    }
    
    ValueType{
        name: "QHelpLink"
        since: [5, 15]
    }
    
    ObjectType{
        name: "QHelpFilterSettingsWidget"
        ModifyFunction{
            signature: "applySettings(QHelpFilterEngine *) const"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "readSettings(const QHelpFilterEngine *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: [5, 15]
    }
    
    ValueType{
        name: "QCompressedHelpInfo"
        since: [5, 13]
    }
    
    ValueType{
        name: "QHelpFilterData"
        since: [5, 13]
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QHelpContentItem::QHelpContentItem', unmatched parameter type 'QHelpDBReader*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'changeEvent(QEvent * event)' in 'QHelpSearchQueryWidget'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'focusInEvent(QFocusEvent * focusEvent)' in 'QHelpSearchQueryWidget'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'changeEvent(QEvent * event)' in 'QHelpSearchResultWidget'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QHelpFilterEngine::setCollectionHandler', unmatched parameter type 'QHelpCollectionHandler*'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QCompressedHelpInfo."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QHelpLink."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QHelpSearchQuery."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QHelpSearchResult."}
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: protected function '*' in final class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: visibility of function '*' modified in class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: hiding of function '*' in class '*'"}
}
