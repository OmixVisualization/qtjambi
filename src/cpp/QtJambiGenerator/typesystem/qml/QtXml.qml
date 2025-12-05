/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    defaultSuperClass: "QtObject"
    qtLibrary: "QtXml"
    module: "qtjambi.xml"
    precompiledHeader: "pch_p.h"
    description: "C++ implementations of SAX and DOM."
    
    ValueType{
        name: "QDomAttr"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomCDATASection"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomCharacterData"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomComment"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomDocument"

        EnumType{
            name: "ParseOption"
            since: [6,5]
        }

        ValueType{
            name: "ParseResult"
            ModifyFunction{
                signature: "operator bool() const"
                rename: "success"
            }
            since: [6,5]
        }
        ModifyFunction{
            signature: "nodeType()const"
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
            signature: "setContent(QIODevice*, QDomDocument::ParseOptions)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcContent"
                    action: ReferenceCount.Set
                }
            }
            since: 6.5
        }
        ModifyFunction{
            signature: "setContent(QXmlStreamReader*, QDomDocument::ParseOptions)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcContent"
                    action: ReferenceCount.Set
                }
            }
            since: 6.5
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
                NoNullPointer{}
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
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QDomDocument$Result::newInstance(%env, jboolean(%in), qtjambi_cast<jstring>(%env, errorMsg), errorLine, errorColumn);"}
                }
            }
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
                NoNullPointer{}
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
                NoNullPointer{}
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
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QDomDocument$Result::newInstance(%env, jboolean(%in), qtjambi_cast<jstring>(%env, errorMsg), errorLine, errorColumn);"}
                }
            }
            Remove{since: [6,5]}
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
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QDomDocument$Result::newInstance(%env, jboolean(%in), qtjambi_cast<jstring>(%env, errorMsg), errorLine, errorColumn);"}
                }
            }
            Remove{since: [6,5]}
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
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtXml::QDomDocument$Result::newInstance(%env, jboolean(%in), qtjambi_cast<jstring>(%env, errorMsg), errorLine, errorColumn);"}
                }
            }
            Remove{since: [6,5]}
        }
    }
    
    ValueType{
        name: "QDomDocumentFragment"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomDocumentType"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomEntity"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomEntityReference"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomImplementation"
        EnumType{
            name: "InvalidDataPolicy"
        }
    }
    
    ValueType{
        name: "QDomNamedNodeMap"
    }
    
    ValueType{
        name: "QDomNode"

        Rejection{fieldName: "impl"}

        EnumType{
            name: "NodeType"
        }

        EnumType{
            name: "EncodingPolicy"
        }
        ModifyFunction{
            signature: "attributes()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomNodeList"
        IteratorType{
            name: "It"
            genericClass: true
            since: 6.9
        }
        Rejection{functionName: "begin"}
        Rejection{functionName: "cbegin"}
        Rejection{functionName: "rbegin"}
        Rejection{functionName: "crbegin"}
        Rejection{functionName: "end"}
        Rejection{functionName: "cend"}
        Rejection{functionName: "rend"}
        Rejection{functionName: "crend"}
    }
    
    ValueType{
        name: "QDomNotation"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomProcessingInstruction"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomText"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDomElement"
        ModifyFunction{
            signature: "nodeType()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setAttribute(const QString &, uint)"
            Remove{
            }
        }
        ModifyFunction{
            signature: "setAttribute(const QString &, qulonglong)"
            Remove{
            }
        }
        ModifyFunction{
            signature: "setAttributeNS(const QString, const QString &, uint)"
            Remove{
            }
        }
        ModifyFunction{
            signature: "setAttributeNS(const QString, const QString &, qulonglong)"
            Remove{
            }
        }
    }
}
