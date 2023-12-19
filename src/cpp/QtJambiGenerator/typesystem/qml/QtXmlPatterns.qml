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
    packageName: "io.qt.xmlpatterns"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtXmlPatterns"
    module: "qtjambi.xmlpatterns"
    description: "Support for XPath, XQuery, XSLT and XML schema validation."
    
    ObjectType{
        name: "QAbstractMessageHandler"
    }
    
    ObjectType{
        name: "QAbstractUriResolver"
    }
    
    ObjectType{
        name: "QAbstractXmlNodeModel"

        Rejection{functionName: "mapToItem"}
        Rejection{functionName: "mapToSequence"}
        Rejection{functionName: "sequencedTypedValue"}
        Rejection{functionName: "sendNamespaces"}
        Rejection{functionName: "iterate"}
        Rejection{functionName: "copyNodeTo"}
        Rejection{functionName: "type"}

        EnumType{name: "NodeCopySetting"}
        EnumType{name: "SimpleAxis"}

        ModifyFunction{
            signature: "createIndex(void *, qint64) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "namespaceForPrefix(QXmlNodeModelIndex,qint16) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "isDeepEqual(QXmlNodeModelIndex,QXmlNodeModelIndex) const"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QAbstractXmlReceiver"

        Rejection{functionName: "item"}
        Rejection{functionName: "sendAsNode"}
        Rejection{fieldName: "d_ptr"}
    }
    
    ValueType{
        name: "QSourceLocation"
    }
    
    ValueType{
        name: "QXmlSchema"
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
        noImplicitConstructors: true
    }
    
    ValueType{
        name: "QXmlName"
        ModifyFunction{
            signature: "QXmlName(qint16,qint16,qint16)"
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
            signature: "setPrefix(qint16)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setNamespaceURI(qint16)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setLocalName(qint16)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "code()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QXmlNamePool"
    }
    
    ValueType{
        name: "QXmlNodeModelIndex"

        Rejection{functionName: "sequencedTypedValue"}
        Rejection{functionName: "sendNamespaces"}
        Rejection{functionName: "iterate"}
        Rejection{functionName: "type"}
        Rejection{enumName: "Axis"}

        EnumType{name: "NodeKind"}
        EnumType{name: "DocumentOrder"}
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
            signature: "namespaceForPrefix(qint16) const"
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
        EnumType{name: "QueryLanguage"}

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
            Delegate{
                name: "set"
                deprecated: true
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
                    modifiedType: "java.util.@Nullable Collection<java.lang.@NonNull String>"
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
        ModifyFunction{
            signature: "setQuery(QUrl,QUrl)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "setQuery(QString,QUrl)"
            ModifyArgument{
                index: 2
                noImplicitCalls: true
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
        ModifyFunction{
            signature: "namePool()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
    }

    Rejection{
        className: "QPatternist::NodeIndexStorage"
    }
    Rejection{
        className: "QPatternist::Item"
    }
    Rejection{
        className: "QPatternist::NamePool"
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
