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
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtHelp"
    module: "qtjambi.help"
    description: "Classes for integrating documentation into applications, similar to Qt Assistant."

    RequiredLibrary{
        name: "QtNetwork"
        mode: RequiredLibrary.ProvideOnly
        platforms: ["linux", "macos"]
    }
    
    Rejection{
        className: "QHelpSearchEngine::SearchHit"
    }
    
    
    EnumType{
        name: "QHelpSearchQuery::FieldName"
    }
    
    ObjectType{
        name: "QHelpContentItem"
    }
    
    
    ObjectType{
        name: "QHelpContentWidget"
    }
    
    ObjectType{
        name: "QHelpIndexWidget"
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "#if QT_VERSION < QT_VERSION_CHECK(6,0,0)\n"+
                          "hash_type qHash(const QMap<QString, QUrl> &value){\n"+
                          "    hash_type hashCode = qHash(int(value.size()));\n"+
                          "    for(const QString& key : value.keys()){\n"+
                          "        hashCode = hashCode * 31 + qHash(key);\n"+
                          "        hashCode = hashCode * 31 + qHash(value.value(key));\n"+
                          "    }\n"+
                          "    return hashCode;\n"+
                          "}\n"+
                          "#else\n"+
                          "hash_type qHash(const QMultiMap<QString, QUrl> &value){\n"+
                          "    hash_type hashCode = qHash(int(value.keys().size()));\n"+
                          "    for(const QString& key : value.keys()){\n"+
                          "        hashCode = hashCode * 31 + qHash(key);\n"+
                          "        hashCode = hashCode * 31 + qHash(value.values(key));\n"+
                          "    }\n"+
                          "    return hashCode;\n"+
                          "}\n"+
                          "#endif"}
        }
    }
    
    ObjectType{
        name: "QHelpIndexModel"
    }
    
    ObjectType{
        name: "QHelpContentModel"
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
    }
    
    ValueType{
        name: "QHelpSearchResult"
        ModifyFunction{
            signature: "operator=(const QHelpSearchResult &)"
            remove: RemoveFlag.All
        }
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
        ModifyFunction{
            signature: "operator=(const QCompressedHelpInfo &)"
            remove: RemoveFlag.All
        }
        since: [5, 13]
    }
    
    ValueType{
        name: "QHelpFilterData"
        ModifyFunction{
            signature: "operator=(const QHelpFilterData &)"
            remove: RemoveFlag.All
        }
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
