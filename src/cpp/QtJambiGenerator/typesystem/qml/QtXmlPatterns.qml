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
    packageName: "io.qt.xmlpatterns"
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtXmlPatterns"
    module: "qtjambi.xmlpatterns"
    description: "Support for XPath, XQuery, XSLT and XML schema validation."
    
    EnumType{
        name: "QXmlNodeModelIndex::NodeKind"
    }
    
    EnumType{
        name: "QXmlNodeModelIndex::DocumentOrder"
    }
    
    EnumType{
        name: "QAbstractXmlNodeModel::NodeCopySetting"
    }
    
    EnumType{
        name: "QAbstractXmlNodeModel::SimpleAxis"
    }
    
    EnumType{
        name: "QXmlQuery::QueryLanguage"
    }
    
    ObjectType{
        name: "QAbstractMessageHandler"
    }
    
    ObjectType{
        name: "QAbstractUriResolver"
    }
    
    
    Rejection{
        className: "QPatternist::NodeIndexStorage"
    }
    
    Rejection{
        className: "QAbstractXmlNodeModel"
        functionName: "mapToItem"
    }
    
    Rejection{
        className: "QAbstractXmlNodeModel"
        functionName: "mapToSequence"
    }
    
    Rejection{
        className: "QAbstractXmlNodeModel"
        functionName: "sequencedTypedValue"
    }
    
    Rejection{
        className: "QAbstractXmlNodeModel"
        functionName: "sendNamespaces"
    }
    
    Rejection{
        className: "QAbstractXmlNodeModel"
        functionName: "iterate"
    }
    
    Rejection{
        className: "QAbstractXmlNodeModel"
        functionName: "copyNodeTo"
    }
    
    Rejection{
        className: "QAbstractXmlNodeModel"
        functionName: "type"
    }
    
    ObjectType{
        name: "QAbstractXmlNodeModel"
        implementing: "java.lang.Cloneable"
        ModifyFunction{
            signature: "createIndex(void *, long long) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "namespaceForPrefix(QXmlNodeModelIndex,short) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "isDeepEqual(QXmlNodeModelIndex,QXmlNodeModelIndex) const"
            remove: RemoveFlag.All
        }
    }
    
    Rejection{
        className: "QAbstractXmlReceiver"
        functionName: "item"
    }
    
    Rejection{
        className: "QAbstractXmlReceiver"
        functionName: "sendAsNode"
    }
    
    Rejection{
        className: "QAbstractXmlReceiver"
        fieldName: "d_ptr"
    }
    
    ObjectType{
        name: "QAbstractXmlReceiver"
    }
    
    ValueType{
        name: "QSourceLocation"
        ModifyFunction{
            signature: "operator=(QSourceLocation)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QXmlSchema"
        ModifyFunction{
            signature: "operator=(const QXmlSchema&)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setMessageHandler(QAbstractMessageHandler*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setNetworkAccessManager(QNetworkAccessManager*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setUriResolver(const QAbstractUriResolver*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QXmlSchemaValidator"
        ModifyFunction{
            signature: "setMessageHandler(QAbstractMessageHandler*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setNetworkAccessManager(QNetworkAccessManager*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setUriResolver(const QAbstractUriResolver*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    Rejection{
        className: "QXmlFormatter"
        functionName: "item"
    }
    
    ObjectType{
        name: "QXmlFormatter"
        ModifyFunction{
            signature: "startFormattingContent()"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QXmlItem"
        ModifyFunction{
            signature: "operator=(QXmlItem)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QXmlName"
        ModifyFunction{
            signature: "operator=(QXmlName)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QXmlName(short,short,short)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "localName()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "prefix()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "hasPrefix()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "hasNamespace()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "namespaceURI()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "isLexicallyEqual(const QXmlName)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setPrefix(const short)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setNamespaceURI(const short)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setLocalName(const short)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "code()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QXmlName(QXmlNamePool&,QString,QString,QString)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.xmlpatterns.QXmlNamePool"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QXmlNamePool& %out = qtjambi_cast<QXmlNamePool&>(%env, %in);"}
                }
            }
        }
    }
    
    ValueType{
        name: "QXmlNamePool"
        ModifyFunction{
            signature: "operator=(QXmlNamePool)"
            remove: RemoveFlag.All
        }
    }
    
    Rejection{
        className: "QXmlNodeModelIndex"
        functionName: "sequencedTypedValue"
    }
    
    Rejection{
        className: "QXmlNodeModelIndex"
        functionName: "sendNamespaces"
    }
    
    Rejection{
        className: "QXmlNodeModelIndex"
        functionName: "iterate"
    }
    
    Rejection{
        className: "QXmlNodeModelIndex"
        functionName: "type"
    }
    
    Rejection{
        className: "QXmlNodeModelIndex"
        enumName: "Axis"
    }
    
    ValueType{
        name: "QXmlNodeModelIndex"
        ModifyFunction{
            signature: "operator=(const QXmlNodeModelIndex &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "name() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "root() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "documentUri() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "baseUri() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "kind() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "isDeepEqual(QXmlNodeModelIndex) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "compareOrder(QXmlNodeModelIndex) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "namespaceBindings() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "namespaceForPrefix(short) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "stringValue() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "is(QXmlNodeModelIndex) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "reset()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "internalPointer()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QXmlSerializerPointer"
    }
    
    ObjectType{
        name: "QXmlQuery"
        ModifyFunction{
            signature: "setFocus(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcFocusDevice"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setNetworkAccessManager(QNetworkAccessManager*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcNetworkAccessManager"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setMessageHandler(QAbstractMessageHandler*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMessageHandler"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setQuery(QIODevice*, QUrl)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setUriResolver(const QAbstractUriResolver*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcUriResolver"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "operator=(const QXmlQuery &)"
            rename: "set"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "evaluateTo(QString*)const"
            Rename{
                to: "evaluateToString"
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString target;\n"+
                                  "QString* %out = &target;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = %in ? qtjambi_cast<jstring>(%env, target) : nullptr;"}
                }
            }
        }
        ModifyFunction{
            signature: "evaluateTo(QAbstractXmlReceiver *)const"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "evaluateTo(QStringList*)const"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.util.Collection<java.lang.String>"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QStringList* %out = qtjambi_cast<QStringList*>(%env, %scope, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "evaluateTo(QIODevice*)const"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "evaluateTo(QXmlResultItems*)const"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
            }
        }
    }
    
    ObjectType{
        name: "QXmlResultItems"
    }
    
    Rejection{
        className: "QXmlSerializer"
        functionName: "item"
    }
    
    ObjectType{
        name: "QXmlSerializer"
        ModifyFunction{
            signature: "atDocumentRoot()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "startContent()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "writeEscaped(QString)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "writeEscapedAttribute(QString)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "write(QString)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "write(QXmlName)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "write(char * const)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "isBindingInScope(QXmlName)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setCodec(const QTextCodec *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcCodec"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QSimpleXmlNodeModel"
        implementing: "java.lang.Cloneable"
        ModifyFunction{
            signature: "namePool()const"
            rename: "namePool_private"
            access: Modification.Private
        }
        ModifyFunction{
            signature: "namePool()const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.xmlpatterns.QXmlNamePool"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = QtJambiAPI::convertNativeToJavaObject(%env, &%in, \"io/qt/xmlpatterns/QXmlNamePool\", false);"}
                }
            }
        }
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: horribly broken type ''"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: template baseclass 'QExplicitlySharedDataPointer<QXmlSerializer>' of 'QXmlSerializerPointer' is not known"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QXmlItem::QXmlItem', unmatched parameter type 'const QPatternist::Item&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QXmlNodeModelIndex::sequencedTypedValue', unmatched return type 'QExplicitlySharedDataPointer<QAbstractXmlForwardIterator<QPatternist::Item>>"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QXmlNamePool::QXmlNamePool', unmatched parameter type 'QPatternist::NamePool*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QXmlNodeModelIndex::QXmlNodeModelIndex', unmatched parameter type 'const QPatternist::NodeIndexStorage&'"}
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QSimpleXmlNodeModel' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QXmlSerializerPointer' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QSourceLocation' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QXmlFormatter' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QAbstractUriResolver' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QAbstractXmlReceiver' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: namespace 'io.qt.xmlpatterns.QXmlNodeModelIndex' for enum 'DocumentOrder' is not declared"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QXmlQuery' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QXmlName' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: namespace 'io.qt.xmlpatterns.QAbstractXmlNodeModel' for enum 'NodeCopySetting' is not declared"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QXmlSerializer' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QAbstractXmlNodeModel' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QXmlItem' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QXmlNodeModelIndex' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QXmlResultItems' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QAbstractMessageHandler' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: namespace 'io.qt.xmlpatterns.QXmlNodeModelIndex' for enum 'NodeKind' is not declared"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: namespace 'io.qt.xmlpatterns.QAbstractXmlNodeModel' for enum 'SimpleAxis' is not declared"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QXmlNamePool' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: namespace 'io.qt.xmlpatterns.QXmlQuery' for enum 'QueryLanguage' is not declared"}
}
