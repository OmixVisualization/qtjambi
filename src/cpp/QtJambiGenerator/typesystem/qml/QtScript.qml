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
    packageName: "io.qt.script"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtScript"
    module: "qtjambi.script"
    description: "Classes for making Qt applications scriptable. Deprecated in favor of the QJS* classes in the Qt QML module."

    Rejection{
        className: "QScriptValueIterator"
    }
    
    Rejection{
        className: "QScriptEngine::FunctionWithArgSignature"
    }
    
    Rejection{
        className: "QScriptEngine"
        functionName: "fromScriptValue"
    }
    
    Rejection{
        className: "QScriptEngine"
        functionName: "toScriptValue"
    }
    
    Rejection{
        className: "QScriptEngine"
        functionName: "newQMetaObject"
    }
    
    Rejection{
        className: "QScriptEngine"
        functionName: "scriptValueFromQMetaObject"
    }
    
    
    Rejection{
        className: "QScriptEngine"
        functionName: "objectById"
    }
    
    Rejection{
        className: "QScriptValue"
        functionName: "toQMetaObject"
    }
    
    Rejection{
        className: "QScriptValueList"
    }
    
    Rejection{
        className: "QScriptValue::QScriptValueList"
    }
    
    Rejection{
        className: "QScriptEngine::DemarshalFunction"
    }
    
    Rejection{
        className: "QScriptEngine::MarshalFunction"
    }
    
    
    EnumType{
        name: "QScriptClass::Extension"
    }
    
    EnumType{
        name: "QScriptClass::QueryFlag"
    }
    
    EnumType{
        name: "QScriptContext::Error"
    }
    
    EnumType{
        name: "QScriptContext::ExecutionState"
    }
    
    EnumType{
        name: "QScriptContextInfo::FunctionType"
    }
    
    EnumType{
        name: "QScriptEngine::QObjectWrapOption"
    }
    
    EnumType{
        name: "QScriptEngine::ValueOwnership"
    }
    
    EnumType{
        name: "QScriptEngineAgent::Extension"
    }
    
    EnumType{
        name: "QScriptValue::PropertyFlag"
    }
    
    EnumType{
        name: "QScriptValue::ResolveFlag"
    }
    
    EnumType{
        name: "QScriptValue::SpecialValue"
    }
    
    EnumType{
        name: "QScriptSyntaxCheckResult::State"
    }
    
    ObjectType{
        name: "QScriptClass"
        ModifyFunction{
            signature: "newIterator(const QScriptValue &)"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "io.qt.script.@NonNull QScriptClassPropertyIterator"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "queryProperty(const QScriptValue &, const QScriptString &, QScriptClass::QueryFlags, uint *)"
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "java.lang.@Nullable Integer"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = %in ? QtJambiAPI::toJavaIntegerObject(%env, int(*%in)) : nullptr;"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "uint %outTmp = uint(QtJambiAPI::fromJavaIntegerObject(%env, %in));\n"+
                                  "uint* %out = %in ? &%outTmp : nullptr;"}
                }
            }
        }

        ModifyFunction{
            signature: "setProperty(QScriptValue&,QScriptString,uint,QScriptValue)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QScriptClassPropertyIterator"
    }
    
    ObjectType{
        name: "QScriptContext"
        ModifyFunction{
            signature: "scopeChain()const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QScriptContextInfo"
    }
    
    ObjectType{
        name: "QScriptEngineAgent"
    }
    
    ValueType{
        name: "QScriptString"
        ModifyFunction{
            signature: "operator QString() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "toArrayIndex ( bool * ) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.Integer"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? QtJambiAPI::toJavaIntegerObject(%env, %in) : nullptr;"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok(false);\n"+
                                  "bool* %out = &ok;"}
                }
            }
        }
    }
    
    ValueType{
        name: "QScriptProgram"
        noImplicitConstructors: true
    }
    
    InterfaceType{
        name: "QScriptExtensionInterface"
    }
    
    ValueType{
        name: "QScriptSyntaxCheckResult"
        CustomConstructor{
            Text{content: "if(copy)\n"+
                          "    return new(placement) QScriptSyntaxCheckResult(*copy);\n"+
                          "else\n"+
                          "    return nullptr;"}
        }
    }
    
    ValueType{
        name: "QScriptValue"
        noImplicitConstructors: true
        ModifyFunction{
            signature: "QScriptValue(uint)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QScriptValue(QScriptEngine*,uint)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QScriptValue(const QLatin1String &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QScriptValue(QScriptEngine * , const char * )"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QScriptValue(const char * )"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "toBoolean () const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "isBoolean () const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "toObject () const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "toUInt32 () const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "isQMetaObject () const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "toUInt16 () const"
            rename: "toShort"
        }
        ModifyFunction{
            signature: "toBool () const"
            rename: "toBoolean"
        }
        ModifyFunction{
            signature: "isBool () const"
            rename: "isBoolean"
        }
        ModifyFunction{
            signature: "toInt32 () const"
            rename: "toInt"
        }
        ModifyFunction{
            signature: "toNumber () const"
            rename: "toDouble"
        }
        ModifyFunction{
            signature: "toInteger () const"
            rename: "private_toInteger"
            access: Modification.Private
        }
        ModifyFunction{
            signature: "setScriptClass(QScriptClass*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "call(const QScriptValue &, const QList<QScriptValue> &)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "java.util.@Nullable Collection<@NonNull QScriptValue>"
                }
                ReplaceDefaultExpression{
                    expression: "new java.util.ArrayList<>()"
                }
            }
        }
        ModifyFunction{
            signature: "construct(const QList<QScriptValue> &)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.util.@Nullable Collection<@NonNull QScriptValue>"
                }
                ReplaceDefaultExpression{
                    expression: "new java.util.ArrayList<>()"
                }
            }
        }
        InjectCode{
            Text{content: "@QtUninvokable\n"+
                          "public final Integer toInteger() {\n"+
                          "    if(Double.isNaN(private_toInteger())){\n"+
                          "        return null;\n"+
                          "    }else{\n"+
                          "        return toInt();\n"+
                          "    }\n"+
                          "}"}
        }
    }
    
    
    InterfaceType{
        name: "QScriptable"
    }
    
    ObjectType{
        name: "QScriptEngine"
        ModifyFunction{
            signature: "setAgent(QScriptEngineAgent*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "QScriptEngine(QObject *)"
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
    
    FunctionalType{
        name: "QScriptEngine::FunctionSignature"
    }
    
    ObjectType{
        name: "QScriptExtensionPlugin"
        ModifyFunction{
            signature: "setupPackage(QString,QScriptEngine*)const"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "QScriptExtensionPlugin(QObject *)"
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
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QHelpContentItem::QHelpContentItem', unmatched parameter type 'QHelpDBReader*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'changeEvent(QEvent * event)' in 'QHelpSearchQueryWidget'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'focusInEvent(QFocusEvent * focusEvent)' in 'QHelpSearchQueryWidget'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'changeEvent(QEvent * event)' in 'QHelpSearchResultWidget'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unhandled enum value: QSysInfo::BigEndian in QAudioFormat::Endian"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unmatched enum QSysInfo::BigEndian"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unhandled enum value: QSysInfo::LittleEndian in QAudioFormat::Endian"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unmatched enum QSysInfo::LittleEndian"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QScriptEngine::newFunction', unmatched parameter type 'QScriptEngine::FunctionWithArgSignature'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QScriptEngine::registerCustomType', unmatched parameter type 'QScriptEngine::MarshalFunction'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QScriptSyntaxCheckResult."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QScriptValue."}
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: Value type 'QScriptSyntaxCheckResult' is missing a default constructor.*"}
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: protected function '*' in final class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: visibility of function '*' modified in class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: hiding of function '*' in class '*'"}
}
