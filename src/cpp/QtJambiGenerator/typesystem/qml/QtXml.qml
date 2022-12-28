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
    packageName: "io.qt.xml"
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtXml"
    module: "qtjambi.xml"
    description: "C++ implementations of SAX and DOM."
    Rejection{
        className: "QXmlAttributes::Attribute"
    }
    
    Rejection{
        className: "QDomNode"
        fieldName: "impl"
    }
    
    EnumType{
        name: "QDomNode::NodeType"
    }
    
    EnumType{
        name: "QDomNode::EncodingPolicy"
    }
    
    EnumType{
        name: "QDomImplementation::InvalidDataPolicy"
    }
    
    ValueType{
        name: "QDomAttr"
        ModifyFunction{
            signature: "operator=(QDomAttr)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomCDATASection"
        ModifyFunction{
            signature: "operator=(QDomCDATASection)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomCharacterData"
        ModifyFunction{
            signature: "operator=(QDomCharacterData)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomComment"
        ModifyFunction{
            signature: "operator=(QDomComment)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomDocument"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(QDomDocument)"
            remove: RemoveFlag.All
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiXml.java"
                quoteAfterLine: "class QDomDocument___"
                quoteBeforeLine: "}// class"
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "setContent(QIODevice*, bool, QString*, int*, int*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcContent"
                    action: ReferenceCount.Set
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString errorMsg;\n"+
                                  "QString *%out = &errorMsg;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorLine = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorLine);"}
                }
            }
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorColumn = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorColumn);"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.xml.QDomDocument$Result"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QDomDocument$Result::newInstance(%env, jboolean(%in), qtjambi_cast<jstring>(%env, errorMsg), errorLine, errorColumn);"}
                }
            }
        }
        ModifyFunction{
            signature: "setContent(QXmlStreamReader*,bool,QString*,int*,int*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcContent"
                    action: ReferenceCount.Set
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString errorMsg;\n"+
                                  "QString *%out = &errorMsg;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorLine = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorLine);"}
                }
            }
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorColumn = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorColumn);"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.xml.QDomDocument$Result"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QDomDocument$Result::newInstance(%env, jboolean(%in), qtjambi_cast<jstring>(%env, errorMsg), errorLine, errorColumn);"}
                }
            }
        }
        ModifyFunction{
            signature: "setContent(QXmlInputSource*,QXmlReader*,QString*,int*,int*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcContent"
                    action: ReferenceCount.Set
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString errorMsg;\n"+
                                  "QString *%out = &errorMsg;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorLine = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorLine);"}
                }
            }
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorColumn = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorColumn);"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.xml.QDomDocument$Result"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QDomDocument$Result::newInstance(%env, jboolean(%in), qtjambi_cast<jstring>(%env, errorMsg), errorLine, errorColumn);"}
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setContent(QXmlInputSource*,bool,QString*,int*,int*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcContent"
                    action: ReferenceCount.Set
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString errorMsg;\n"+
                                  "QString *%out = &errorMsg;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorLine = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorLine);"}
                }
            }
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorColumn = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorColumn);"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.xml.QDomDocument$Result"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QDomDocument$Result::newInstance(%env, jboolean(%in), qtjambi_cast<jstring>(%env, errorMsg), errorLine, errorColumn);"}
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setContent(QByteArray, bool, QString*, int*, int*)"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString errorMsg;\n"+
                                  "QString *%out = &errorMsg;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorLine = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorLine);"}
                }
            }
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorColumn = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorColumn);"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.xml.QDomDocument$Result"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QDomDocument$Result::newInstance(%env, jboolean(%in), qtjambi_cast<jstring>(%env, errorMsg), errorLine, errorColumn);"}
                }
            }
        }
        ModifyFunction{
            signature: "setContent(QString, bool, QString*, int*, int*)"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString errorMsg;\n"+
                                  "QString *%out = &errorMsg;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorLine = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorLine);"}
                }
            }
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorColumn = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorColumn);"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.xml.QDomDocument$Result"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QDomDocument$Result::newInstance(%env, jboolean(%in), qtjambi_cast<jstring>(%env, errorMsg), errorLine, errorColumn);"}
                }
            }
        }
        ModifyFunction{
            signature: "setContent(QString,QString*,int*,int*)"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString errorMsg;\n"+
                                  "QString *%out = &errorMsg;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorLine = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorLine);"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorColumn = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorColumn);"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.xml.QDomDocument$Result"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QDomDocument$Result::newInstance(%env, jboolean(%in), qtjambi_cast<jstring>(%env, errorMsg), errorLine, errorColumn);"}
                }
            }
        }
        ModifyFunction{
            signature: "setContent(QByteArray,QString*,int*,int*)"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString errorMsg;\n"+
                                  "QString *%out = &errorMsg;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorLine = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorLine);"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorColumn = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorColumn);"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.xml.QDomDocument$Result"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QDomDocument$Result::newInstance(%env, jboolean(%in), qtjambi_cast<jstring>(%env, errorMsg), errorLine, errorColumn);"}
                }
            }
        }
        ModifyFunction{
            signature: "setContent(QIODevice*,QString*,int*,int*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcContent"
                    action: ReferenceCount.Set
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString errorMsg;\n"+
                                  "QString *%out = &errorMsg;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorLine = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorLine);"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint errorColumn = 0;\n"+
                                  "int *%out = reinterpret_cast<int *>(&errorColumn);"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.xml.QDomDocument$Result"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QDomDocument$Result::newInstance(%env, jboolean(%in), qtjambi_cast<jstring>(%env, errorMsg), errorLine, errorColumn);"}
                }
            }
        }
    }
    
    ValueType{
        name: "QDomDocumentFragment"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(QDomDocumentFragment)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomDocumentType"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(QDomDocumentType)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomEntity"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(QDomEntity)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomEntityReference"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(QDomEntityReference)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomImplementation"
        ModifyFunction{
            signature: "operator=(QDomImplementation)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomNamedNodeMap"
        ModifyFunction{
            signature: "operator=(QDomNamedNodeMap)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomNode"
        ModifyFunction{
            signature: "operator=(QDomNode)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "attributes()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomNodeList"
        ModifyFunction{
            signature: "operator=(const QDomNodeList &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomNotation"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(QDomNotation)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomProcessingInstruction"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(QDomProcessingInstruction)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomText"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(const QDomText &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QXmlParseException"
    }
    
    ValueType{
        name: "QXmlAttributes"
        ModifyFunction{
            signature: "index(const QLatin1String&) const"
            Remove{
            }
        }
        ModifyFunction{
            signature: "value(const QLatin1String&) const"
            Remove{
            }
        }
        ModifyFunction{
            signature: "operator=(const QXmlAttributes &)"
            remove: RemoveFlag.All
            since: [5, 12]
        }
    }
    
    ValueType{
        name: "QXmlNamespaceSupport"
        CustomConstructor{
            Text{content: "QXmlNamespaceSupport *clone = new(placement) QXmlNamespaceSupport;\n"+
                          "if(copy){\n"+
                          "    clone->setPrefix(\"\", copy->uri(\"\"));\n"+
                          "\n"+
                          "    QStringList prefixes = copy->prefixes();\n"+
                          "    for (int i=0; i<prefixes.size(); ++i)\n"+
                          "        clone->setPrefix(prefixes.at(i), copy->uri(prefixes.at(i)));\n"+
                          "}\n"+
                          "return clone;"}
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiXml.java"
                quoteAfterLine: "class QXmlNamespaceSupport___"
                quoteBeforeLine: "}// class"
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "processName(QString,bool,QString&,QString&)const"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %out;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %out;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.xml.QXmlNamespaceSupport$ProcessedName"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QXmlNamespaceSupport$ProcessedName::newInstance(%env, qtjambi_cast<jstring>(%env, __qt_%3), qtjambi_cast<jstring>(%env, __qt_%4));"}
                }
            }
        }
        ModifyFunction{
            signature: "splitName(QString,QString&,QString&)const"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %out;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %out;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.xml.QXmlNamespaceSupport$SplitName"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QXmlNamespaceSupport$SplitName::newInstance(%env, qtjambi_cast<jstring>(%env, __qt_%2), qtjambi_cast<jstring>(%env, __qt_%3));"}
                }
            }
        }
    }
    
    ValueType{
        name: "QDomElement"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(QDomElement)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setAttribute(const QString &, unsigned int)"
            Remove{
            }
        }
        ModifyFunction{
            signature: "setAttribute(const QString &, unsigned long long)"
            Remove{
            }
        }
        ModifyFunction{
            signature: "setAttributeNS(const QString, const QString &, unsigned int)"
            Remove{
            }
        }
        ModifyFunction{
            signature: "setAttributeNS(const QString, const QString &, unsigned long long)"
            Remove{
            }
        }
    }
    
    InterfaceType{
        name: "QXmlContentHandler"
        ModifyFunction{
            signature: "setDocumentLocator(QXmlLocator*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    InterfaceType{
        name: "QXmlDTDHandler"
    }
    
    InterfaceType{
        name: "QXmlDeclHandler"
    }
    
    InterfaceType{
        name: "QXmlEntityResolver"
        InjectCode{
            target: CodeClass.JavaInterface
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiXml.java"
                quoteAfterLine: "interface QXmlEntityResolver___"
                quoteBeforeLine: "}// class"
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "resolveEntity(QString,QString,QXmlInputSource&*)"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "// nothing"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QXmlInputSource *%out = nullptr;"}
                }
            }
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "io.qt.xml.QXmlEntityResolver$ResolvedEntity"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject java_inputSource = qtjambi_cast<jobject>(%env, __qt_%3);\n"+
                                  "jobject %out = Java::QtXml::QXmlEntityResolver$ResolvedEntity::newInstance(%env, !%in, java_inputSource);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "bool %out = !Java::QtXml::QXmlEntityResolver$ResolvedEntity::error(%env, %in);\n"+
                                  "jobject __inputSource = Java::QtXml::QXmlEntityResolver$ResolvedEntity::inputSource(%env, %in);\n"+
                                  "%3 = qtjambi_cast<QXmlInputSource*>(%env, __inputSource);"}
                }
            }
        }
    }
    
    InterfaceType{
        name: "QXmlErrorHandler"
    }
    
    InterfaceType{
        name: "QXmlLexicalHandler"
    }
    
    ObjectType{
        name: "QXmlDefaultHandler"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "setDocumentLocator(QXmlLocator*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "QXmlInputSource"
    }
    
    ObjectType{
        name: "QXmlLocator"
    }
    
    ObjectType{
        name: "QXmlSimpleReader"
        ModifyFunction{
            signature: "parse(const QXmlInputSource*,bool)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "parse(const QXmlInputSource*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "parse(const QXmlInputSource&)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setContentHandler(QXmlContentHandler*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcContentHandler"
                    declareVariable: "QXmlReader"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setDTDHandler(QXmlDTDHandler*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcDTDHandler"
                    declareVariable: "QXmlReader"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setDeclHandler(QXmlDeclHandler*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcDeclHandler"
                    declareVariable: "QXmlReader"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setEntityResolver(QXmlEntityResolver*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcEntityResolver"
                    declareVariable: "QXmlReader"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setErrorHandler(QXmlErrorHandler*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcErrorHandler"
                    declareVariable: "QXmlReader"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setLexicalHandler(QXmlLexicalHandler*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcLexicalHandler"
                    declareVariable: "QXmlReader"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QXmlReader"
        ModifyFunction{
            signature: "parse(const QXmlInputSource*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "parse(const QXmlInputSource&)"
            remove: RemoveFlag.JavaAndNative
        }
        ModifyFunction{
            signature: "setContentHandler(QXmlContentHandler*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcContentHandler"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setDTDHandler(QXmlDTDHandler*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcDTDHandler"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setDeclHandler(QXmlDeclHandler*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcDeclHandler"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setEntityResolver(QXmlEntityResolver*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcEntityResolver"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setErrorHandler(QXmlErrorHandler*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcErrorHandler"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setLexicalHandler(QXmlLexicalHandler*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                ReferenceCount{
                    variableName: "__rcLexicalHandler"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "parse(const QXmlInputSource &)"
            Remove{
                codeClass: RemoveFlag.JavaAndNative
            }
            InjectCode{
                target: CodeClass.Shell
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiXml.java"
                    quoteAfterLine: "class QXmlReader_parse___"
                    quoteBeforeLine: "}// class"
                }
            }
        }
        ModifyFunction{
            signature: "property(QString,bool*)const"
            Remove{
                codeClass: RemoveFlag.JavaAndNative
            }
        }
        ModifyFunction{
            signature: "setProperty(QString,void*)"
            Remove{
                codeClass: RemoveFlag.JavaAndNative
            }
        }
        ModifyFunction{
            signature: "hasProperty(QString)const"
            Remove{
                codeClass: RemoveFlag.JavaAndNative
            }
        }
        ModifyFunction{
            signature: "feature(QString,bool*)const"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "// nothing"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok = false;\n"+
                                  "bool *%out = &ok;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.Boolean"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "bool %out = false;\n"+
                                  "if(%in){\n"+
                                  "    if (%2) *%2 = true;\n"+
                                  "    %out = QtJambiAPI::fromJavaBooleanObject(%env, %in);\n"+
                                  "}"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = ok ? QtJambiAPI::toJavaBooleanObject(%env, %in) : nullptr;"}
                }
            }
        }
    }
}
