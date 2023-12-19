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
    packageName: "io.qt.qml"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtQml"
    module: "qtjambi.qml"
    description: "Classes for QML and JavaScript languages."
    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        Text{content: "void initialize_meta_info_QQmlListProperty();"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        Text{content: "initialize_meta_info_QQmlListProperty();"}
    }
    
    InjectCode{
        target: CodeClass.Java
        position: Position.End
        Text{content: "internal.setQmlClassInfoGeneratorFunction(QmlClassInfoProvider::provide);"}
    }
    
    InjectCode{
        target: CodeClass.ModuleInfo
        Text{content: "exports io.qt.qml.util;"}
    }
    
    Rejection{
        className: "QQmlTriviallyDestructibleDebuggingEnabler"
    }
    
    Rejection{
        className: ""
        functionName: "qmlCreateCustomParser"
    }
    
    Rejection{
        className: ""
        functionName: "qjsvalue_cast_helper"
    }
    
    Rejection{
        className: "QQmlTypeNotAvailable"
    }
    
    Rejection{
        className: "QQmlInfo"
        functionName: "operator<<"
    }
    
    Rejection{
        className: ""
        functionName: "qjsvalue_cast"
    }
    Rejection{
        className: "QQmlTypeNotAvailable"
        since: 6.6
    }
    
    Rejection{
        className: "QJSValueList"
    }

    Rejection{
        className: "QJSList"
    }

    Rejection{
        className: "QJSListIndexClamp"
    }
    
    Rejection{
        className: "QJSValue::QJSValueList"
    }
    
    Rejection{
        className: "QQmlTypeInfo"
    }
    
    Rejection{
        className: "QQmlListProperty"
    }
    
    Rejection{
        className: "QQmlV4Function"
    }
    
    Rejection{
        className: "QV4::CompiledData::CompilationUnit"
    }
    
    Rejection{
        className: "QV4::CompiledData"
    }
    
    Rejection{
        className: "QV4"
    }
    
    Rejection{
        className: "QQmlModuleRegistration"
    }
    
    Rejection{
        className: ""
        enumName: "QQmlModuleImportSpecialVersions"
    }
    
    Rejection{
        className: "QmlTypeAndRevisionsRegistration"
    }
    
    Rejection{
        className: "QV4::ExecutionEngine"
    }
    
    Rejection{
        className: "QQmlComponentAttached"
    }
    
    EnumType{
        name: "QJSValue::ObjectConversionBehavior"
        since: [6, 1]
    }
    
    EnumType{
        name: "QQmlIncubator::IncubationMode"
    }
    
    EnumType{
        name: "QQmlIncubator::Status"
    }
    
    EnumType{
        name: "QQmlProperty::Type"
    }
    
    EnumType{
        name: "QQmlProperty::PropertyTypeCategory"
    }
    
    EnumType{
        name: "QJSValue::SpecialValue"
    }
    
    EnumType{
        name: "QJSValue::ErrorType"
    }
    
    EnumType{
        name: "QQmlComponent::CompilationMode"
    }
    
    EnumType{
        name: "QQmlComponent::Status"
    }
    
    EnumType{
        name: "QQmlAbstractUrlInterceptor::DataType"
    }
    
    InterfaceType{
        name: "QQmlAbstractUrlInterceptor"
    }
    
    EnumType{
        name: "QQmlDebuggingEnabler::StartMode"
    }
    
    ObjectType{
        name: "QQmlDebuggingEnabler"
        ModifyFunction{
            signature: "QQmlDebuggingEnabler(bool)"
            remove: RemoveFlag.All
        }
    }
    
    EnumType{
        name: "QQmlFile::Status"
    }
    
    ObjectType{
        name: "QQmlFile"
        ModifyFunction{
            signature: "data() const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QQmlContext::PropertyPair"
    }
    
    ObjectType{
        name: "QQmlContext"
        ModifyFunction{
            signature: "nameForObject(QObject*) const"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "nameForObject(const QObject*) const"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "setContextObject(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setContextProperty(QString,QObject*)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ValueType{
        name: "QQmlScriptString"
        ModifyFunction{
            signature: "numberLiteral( bool * ) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.@Nullable Double"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? QtJambiAPI::toJavaDoubleObject(%env, %in) : nullptr;"}
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
        ModifyFunction{
            signature: "booleanLiteral( bool * ) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.@Nullable Boolean"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? QtJambiAPI::toJavaBooleanObject(%env, %in) : nullptr;"}
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
    
    ObjectType{
        name: "QJSManagedValue"
        EnumType{
            name: "Type"
            since: [6, 1]
        }
        ModifyFunction{
            signature: "QJSManagedValue(QJSValue, QJSEngine *)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "QJSManagedValue(QJSPrimitiveValue, QJSEngine *)"
            noImplicitArguments: true
        }
        since: [6, 1]
    }
    
    ValueType{
        name: "QJSPrimitiveValue"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/QmlAPI"
                location: Include.Global
            }
        }
        EnumType{
            name: "Type"
            since: [6, 1]
        }
        ModifyFunction{
            signature: "QJSPrimitiveValue(QJSPrimitiveNull)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QJSPrimitiveValue(const QMetaType, const void *)"
            remove: RemoveFlag.All
            since: [6, 4]
        }
        ModifyFunction{
            signature: "QJSPrimitiveValue(QJSPrimitiveUndefined)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator--(int)"
            remove: RemoveFlag.All
            since: [6, 2]
        }
        ModifyFunction{
            signature: "operator++(int)"
            remove: RemoveFlag.All
            since: [6, 2]
        }
        ModifyFunction{
            signature: "operator*(QJSPrimitiveValue)"
            Delegate{name: "multiplied"; deprecated: true}
        }
        ModifyFunction{
            signature: "operator%(QJSPrimitiveValue)"
            Delegate{name: "modulo"; deprecated: true}
        }
        ModifyFunction{
            signature: "operator+(QJSPrimitiveValue)"
            Delegate{name: "added"; deprecated: true}
        }
        ModifyFunction{
            signature: "operator-(QJSPrimitiveValue)"
            Delegate{name: "substracted"; deprecated: true}
        }
        ModifyFunction{
            signature: "operator/(QJSPrimitiveValue)"
            Delegate{name: "divided"; deprecated: true}
        }
        ModifyFunction{
            signature: "operate<Operators>(QJSPrimitiveValue,QJSPrimitiveValue)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operateOnIntegers<Operators,Lhs,Rhs>(QJSPrimitiveValue,QJSPrimitiveValue)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "data()"
            remove: RemoveFlag.All
            since: 6.6
        }
        ModifyFunction{
            signature: "data()const"
            remove: RemoveFlag.All
            since: [6, 6]
        }
        ModifyFunction{
            signature: "constData()const"
            remove: RemoveFlag.All
            since: [6, 6]
        }
        since: [6, 1]
    }
    
    ValueType{
        name: "QJSPrimitiveNull"
        generate: false
        since: [6, 1]
    }
    
    ValueType{
        name: "QJSPrimitiveUndefined"
        generate: false
        since: [6, 1]
    }
    
    ObjectType{
        name: "QJSNumberCoercion"
        since: [6, 1]
    }
    
    ValueType{
        name: "QJSValue"
        ModifyFunction{
            signature: "QJSValue(uint)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QJSValue(const QLatin1String &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QJSValue(const char * )"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "toUInt() const"
            remove: RemoveFlag.All
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
            signature: "isVariant () const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "toNumber () const"
            rename: "toDouble"
        }
        ModifyFunction{
            signature: "toVariant () const"
            rename: "toObject"
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQml.java"
                quoteAfterLine: "class __QJSValue"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "call(const QList<QJSValue> &)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "java.util.Collections.emptyList()"
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "callWithInstance(const QJSValue &, const QList<QJSValue> &)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "java.util.Collections.emptyList()"
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "callAsConstructor(const QList<QJSValue> &)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "java.util.Collections.emptyList()"
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "call(const QList<QJSValue> &)const"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "java.util.Collections.emptyList()"
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "callWithInstance(const QJSValue &, const QList<QJSValue> &)const"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "java.util.Collections.emptyList()"
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "callAsConstructor(const QList<QJSValue> &)const"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "java.util.Collections.emptyList()"
                }
            }
            since: 6
        }
    }
    
    ValueType{
        name: "QQmlInfo"
        CustomConstructor{
            Text{content: "if(copy){\n"+
                          "    return new(placement) QQmlInfo(*copy);\n"+
                          "}else{\n"+
                          "    return new(placement) QQmlInfo(qmlDebug(nullptr));\n"+
                          "}"}
        }
        CustomConstructor{
            type: CustomConstructor.Default
            Text{content: "new(placement) QQmlInfo(qmlDebug(nullptr));"}
        }
        ModifyFunction{
            signature: "operator<<(QQmlInfo,const QWindow*)"
            rename: "append"
            since: 6
        }
    }
    
    ValueType{
        name: "QQmlError"
        ModifyFunction{
            signature: "setObject(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QJSValueIterator"
        ModifyFunction{
            signature: "operator= ( QJSValue & )"
            remove: RemoveFlag.All
        }
    }
    
    NamespaceType{
        name: "QtQml"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        Rejection{functionName: "qmlInfo"}
        ModifyFunction{
            signature: "qmlAttachedPropertiesObject(int *, const QObject *, const QMetaObject *, bool)"
            remove: RemoveFlag.All
            until: 5
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQml.java"
                quoteAfterLine: "class __QtQml__"
                quoteBeforeLine: "}// class"
            }
        }

        FunctionalType{
            name: "QQmlAttachedPropertiesFunc"
            ExtraIncludes{
                Include{
                    fileName: "hashes.h"
                    location: Include.Local
                }
            }
        }

        FunctionalType{
            name: "ValueCallback"
            ExtraIncludes{
                Include{
                    fileName: "QtQml/QQmlEngine"
                    location: Include.Global
                }
                Include{
                    fileName: "QtQml/QJSEngine"
                    location: Include.Global
                }
                Include{
                    fileName: "hashes.h"
                    location: Include.Local
                }
            }
        }

        FunctionalType{
            name: "ObjectCallback"
            ExtraIncludes{
                Include{
                    fileName: "QtQml/QQmlEngine"
                    location: Include.Global
                }
                Include{
                    fileName: "QtQml/QJSEngine"
                    location: Include.Global
                }
                Include{
                    fileName: "hashes.h"
                    location: Include.Local
                }
            }
        }
    }
    
    ObjectType{
        name: "QJSEngine"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        Rejection{className: "FunctionWithArgSignature"}
        Rejection{className: "FunctionSignature"}
        Rejection{functionName: "registerCustomType"}
        Rejection{functionName: "scriptValueFromQMetaObject"}
        Rejection{functionName: "newFunction"}
        Rejection{functionName: "objectById"}
        EnumType{
            name: "ObjectOwnership"
            RenameEnumValue{
                name: "CppOwnership"
                rename: "JavaOwnership"
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: String.raw`/**
 * @deprecated The native name of this enum entry is misleading. Use {@link ObjectOwnership#JavaOwnership} instead.
 */
@Deprecated
public static final ObjectOwnership CppOwnership = JavaOwnership;
/**
 * Equivalent to {@link ObjectOwnership#JavaOwnership}.
 */
public static final ObjectOwnership KotlinOwnership = JavaOwnership;`}
            }

            since: 6
        }
        EnumType{
            name: "Extension"
        }

        ModifyFunction{
            signature: "newQMetaObject<T>()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "toManagedValue<T>(T)"
            Instantiation{
                proxyCall: "qtjambi_toManagedValue"
                Argument{
                    type: "QVariant"
                }
                AddTypeParameter{
                    name: "T"
                }
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "T"
                    }
                }
            }
            since: [6, 1]
        }
        ModifyFunction{
            signature: "fromManagedValue<T>(QJSManagedValue)"
            Instantiation{
                proxyCall: "qtjambi_fromManagedValue"
                Argument{
                    type: "QVariant"
                }
                AddTypeParameter{
                    name: "T"
                }
                AddArgument{
                    name: "type"
                    type: "io.qt.core.QMetaType"
                }
                ModifyArgument{
                    index: 0
                    replaceType: "T"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = %in;"}
                    }
                }
            }
            since: [6, 1]
        }
        ModifyFunction{
            signature: "toScriptValue<T>(T)"
            Instantiation{
                proxyCall: "qtjambi_toScriptValue"
                Argument{
                    type: "QVariant"
                }
                AddTypeParameter{
                    name: "T"
                }
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "T"
                    }
                }
            }
        }
        ModifyFunction{
            signature: "fromScriptValue<T>(QJSValue)"
            Instantiation{
                proxyCall: "qtjambi_fromScriptValue"
                Argument{
                    type: "QVariant"
                }
                AddTypeParameter{
                    name: "T"
                }
                AddArgument{
                    name: "type"
                    type: "io.qt.core.QMetaType"
                }
                ModifyArgument{
                    index: 0
                    replaceType: "T"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = %in;"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "toPrimitiveValue<T>(T)"
            Instantiation{
                Argument{
                    type: "int"
                }
            }
            Instantiation{
                Argument{
                    type: "bool"
                }
            }
            Instantiation{
                Argument{
                    type: "double"
                }
            }
            Instantiation{
                Argument{
                    type: "QString"
                }
            }
        }
        ModifyFunction{
            signature: "fromPrimitiveValue<T>(QJSPrimitiveValue)"
            remove: RemoveFlag.All
            since: 6.5
        }
        ModifyFunction{
            signature: "fromVariant<T>(QVariant)"
            Instantiation{
                proxyCall: "qtjambi_fromVariant"
                Argument{
                    type: "QVariant"
                }
                AddTypeParameter{
                    name: "T"
                }
                AddArgument{
                    name: "type"
                    type: "io.qt.core.QMetaType"
                }
                ModifyArgument{
                    index: 0
                    replaceType: "T"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = %in;"}
                    }
                }
            }
            since: [6, 3]
        }
        ModifyFunction{
            signature: "coerceValue<From,To>(From)"
            remove: RemoveFlag.All
            since: 6.5
        }

        ModifyFunction{
            signature: "setObjectOwnership(QObject*,QJSEngine::ObjectOwnership)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if (%1 != null && %2 != null){\n"+
                              "    switch(%2){\n"+
                              "    case JavaOwnership:\n"+
                              "        if(%1.parent()==null){\n"+
                              "            QtJambi_LibraryUtilities.internal.setJavaOwnership(%1);\n"+
                              "        }\n"+
                              "        break;\n"+
                              "    default:\n"+
                              "        QtJambi_LibraryUtilities.internal.setCppOwnership(%1);\n"+
                              "        break;\n"+
                              "    }\n"+
                              "}"}
            }
            since: 6
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQml.java"
                quoteAfterLine: "class QJSEngine__"
                quoteBeforeLine: "}// class"
            }
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQml.java"
                quoteAfterLine: "class QJSEngine_6_"
                quoteBeforeLine: "}// class"
                since: 6
            }
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQml.java"
                quoteAfterLine: "class QJSEngine_61_"
                quoteBeforeLine: "}// class"
                since: [6, 1]
            }
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQml.java"
                quoteAfterLine: "class QJSEngine_63_"
                quoteBeforeLine: "}// class"
                since: [6, 3]
            }
        }
    }
    
    ObjectType{
        name: "QQmlEngine"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/QmlAPI"
                location: Include.Global
            }
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
        EnumType{
            name: "ObjectOwnership"
            RenameEnumValue{
                name: "CppOwnership"
                rename: "JavaOwnership"
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: String.raw`/**
 * @deprecated The native name of this enum entry is misleading. Use {@link ObjectOwnership#JavaOwnership} instead.
 */
@Deprecated
public static final ObjectOwnership CppOwnership = JavaOwnership;
/**
 * Equivalent to {@link ObjectOwnership#JavaOwnership}.
 */
public static final ObjectOwnership KotlinOwnership = JavaOwnership;`}
            }

            until: 5
        }
        ModifyFunction{
            signature: "addImageProvider(QString,QQmlImageProviderBase*)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setIncubationController(QQmlIncubationController*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcIncubationController"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setNetworkAccessManagerFactory(QQmlNetworkAccessManagerFactory*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcNetworkAccessManagerFactory"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setContextForObject(QObject*,QQmlContext*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setObjectOwnership(QObject*,QQmlEngine::ObjectOwnership)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if (%1 != null && %2 != null){\n"+
                              "switch(%2){\n"+
                              "    case JavaOwnership:\n"+
                              "        if(%1.parent()==null){\n"+
                              "            QtJambi_LibraryUtilities.internal.setJavaOwnership(%1);\n"+
                              "        }\n"+
                              "        break;\n"+
                              "    default:\n"+
                              "        QtJambi_LibraryUtilities.internal.setCppOwnership(%1);\n"+
                              "        break;\n"+
                              "    }\n"+
                              "}"}
            }
            until: 5
        }
        ModifyFunction{
            signature: "setUrlInterceptor(QQmlAbstractUrlInterceptor*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcUrlInterceptor"
                    action: ReferenceCount.Add
                    since: 6
                }
                ReferenceCount{
                    variableName: "__rcUrlInterceptor"
                    action: ReferenceCount.Set
                    until: 5
                }
            }
        }
        ModifyFunction{
            signature: "addUrlInterceptor(QQmlAbstractUrlInterceptor*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcUrlInterceptor"
                    action: ReferenceCount.Add
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "removeUrlInterceptor(QQmlAbstractUrlInterceptor*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcUrlInterceptor"
                    action: ReferenceCount.Take
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "singletonInstance(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "singletonInstance<T>(int)"
            Instantiation{
                Argument{
                    type: "QJSValue"
                    isImplicit: false
                }
            }
        }
        ModifyFunction{
            signature: "singletonInstance(QAnyStringView,QAnyStringView)"
            remove: RemoveFlag.All
            since: [6,5]
        }
        ModifyFunction{
            signature: "singletonInstance<T>(QAnyStringView,QAnyStringView)"
            Instantiation{
                Argument{
                    type: "QJSValue"
                    isImplicit: false
                }
            }
            since: [6,5]
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQml.java"
                quoteAfterLine: "class QQmlEngine__"
                quoteBeforeLine: "}// class"
            }
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQml.java"
                quoteAfterLine: "class QQmlEngine_65_"
                quoteBeforeLine: "}// class"
                since: [6,5]
            }
        }
    }
    
    ObjectType{
        name: "QQmlApplicationEngine"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/QmlAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QQmlApplicationEngine(QUrl,QObject*)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "rootObjects()"
            remove: RemoveFlag.All
            since: [5, 9]
            until: 5
        }
        ModifyFunction{
            signature: "loadData(const QByteArray &, const QUrl &)"
            blockExceptions: true
        }
    }
    
    ObjectType{
        name: "QQmlExtensionPlugin"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QScopedPointer"
                location: Include.Global
            }
            Include{
                fileName: "QtCore/QByteArray"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "QQmlEngineExtensionPlugin"
        since: [5, 15]
    }
    
    ObjectType{
        name: "QQmlComponent"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/QmlAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QQmlComponent(QQmlEngine*,QUrl,QObject*)"
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "QQmlComponent(QQmlEngine*,QUrl,QQmlComponent::CompilationMode,QObject*)"
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "setInitialProperties(QObject*,QMap<QString,QVariant>)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQml.java"
                quoteAfterLine: "class __QQmlComponent"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "create(QQmlContext *)"
            blockExceptions: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createWithInitialProperties(QMap<QString,QVariant>,QQmlContext*)"
            blockExceptions: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            since: [5, 14]
        }
        ModifyFunction{
            signature: "createObject(QObject*,QMap<QString,QVariant>)"
            blockExceptions: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            since: [6, 3]
        }
        ModifyFunction{
            signature: "create(QQmlIncubator &, QQmlContext *, QQmlContext *)"
            blockExceptions: true
        }
        ModifyFunction{
            signature: "setData(QByteArray,QUrl)"
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
        }
    }
    
    ObjectType{
        name: "QQmlExpression"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "evaluate(bool *)"
            throwing: "ValueIsUndefined"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool valueIsUndefined = false;\n"+
                                  "bool* %out = &valueIsUndefined;"}
                }
            }
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "if(valueIsUndefined){\n"+
                                  "    Java::QtQml::QQmlExpression$ValueIsUndefined::throwNew(%env, \"Value is undefined.\" QTJAMBI_STACKTRACEINFO );\n"+
                                  "}else{\n"+
                                  "    %out = qtjambi_cast<jobject>(%env, %in);\n"+
                                  "}"}
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQml.java"
                quoteAfterLine: "class QQmlExpression__"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    EnumType{
        name: "QQmlImageProviderBase::Flag"
    }
    
    EnumType{
        name: "QQmlImageProviderBase::ImageType"
    }
    
    InterfaceType{
        name: "QQmlImageProviderBase"
        until: 5
    }
    
    ObjectType{
        name: "QQmlImageProviderBase"
        since: 6
    }
    
    InterfaceType{
        name: "QQmlIncubationController"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        InjectCode{
            target: CodeClass.JavaInterface
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQml.java"
                quoteAfterLine: "class QQmlIncubationController__"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "incubateWhile(bool *, int)"
            blockExceptions: true
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.qml.QQmlIncubationController$WhileFlag"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jlong ptr = Java::QtQml::QQmlIncubationController$WhileFlag::flag(%env, %in);\n"+
                                  "volatile bool* %out = reinterpret_cast<volatile bool* >(ptr);"}
                }
                ReferenceCount{
                    variableName: "__rcWhileFlag"
                    action: ReferenceCount.Set
                }
            }
            until: [5, 14]
        }
        ModifyFunction{
            signature: "incubateWhile(bool *, int)"
            remove: RemoveFlag.All
            since: [5, 15]
            until: 5
        }
        ModifyFunction{
            signature: "incubateWhile(std::atomic<bool> *, int)"
            blockExceptions: true
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.qml.QQmlIncubationController$WhileFlag"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jlong ptr = Java::QtQml::QQmlIncubationController$WhileFlag::flag(%env, %in);\n"+
                                  "std::atomic<bool>* %out = reinterpret_cast<std::atomic<bool>* >(ptr);"}
                }
                ReferenceCount{
                    variableName: "__rcWhileFlag"
                    action: ReferenceCount.Set
                }
            }
            since: [5, 15]
        }
        ModifyFunction{
            signature: "incubateFor(int)"
            blockExceptions: true
        }
    }
    
    ObjectType{
        name: "QQmlIncubator"
        ModifyFunction{
            signature: "setInitialState(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "forceCompletion()"
            blockExceptions: true
        }
        ModifyFunction{
            signature: "object() const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    
    ValueType{
        name: "QQmlListReference"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QScopedPointer"
                location: Include.Global
            }
            Include{
                fileName: "QtCore/QByteArray"
                location: Include.Global
            }
        }
        InjectCode{
            target: CodeClass.Native
            Text{content: "QT_WARNING_DISABLE_DEPRECATED\nQT_WARNING_DISABLE_GCC(\"-Wdeprecated-declarations\")"}
        }
    }
    
    InterfaceType{
        name: "QQmlNetworkAccessManagerFactory"
        ModifyFunction{
            signature: "create(QObject *)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    InterfaceType{
        name: "QQmlParserStatus"
    }
    
    InterfaceType{
        name: "QQmlTypesExtensionInterface"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QScopedPointer"
                location: Include.Global
            }
            Include{
                fileName: "QtCore/QByteArray"
                location: Include.Global
            }
        }
    }
    
    InterfaceType{
        name: "QQmlExtensionInterface"
    }
    
    InterfaceType{
        name: "QQmlEngineExtensionInterface"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/QmlAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "initializeEngine(QQmlEngine *, const char *)"
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
        name: "QQmlProperty"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QScopedPointer"
                location: Include.Global
            }
            Include{
                fileName: "QtCore/QByteArray"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "connectNotifySignal(QObject *, int) const"
            access: Modification.Private
        }
        ModifyFunction{
            signature: "connectNotifySignal(QObject *, const char *) const"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 2
                    metaName: "slot"
                }
                ArgumentMap{
                    index: 1
                    metaName: "dest"
                }
                Text{content: "if(slot!=null && !slot.startsWith(\"1\") && !slot.startsWith(\"2\")) {\n"+
                              "    io.qt.core.QMetaMethod method = dest.metaObject().method(slot);\n"+
                              "    if(method!=null && method.isValid()) {\n"+
                              "        if(method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)\n"+
                              "            slot = \"2\" + method.cppMethodSignature();\n"+
                              "        else\n"+
                              "            slot = \"1\" + method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}"}
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiQml.java"
                quoteAfterLine: "class QQmlProperty__"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QQmlPropertyMap"
        ModifyFunction{
            signature: "QQmlPropertyMap<DerivedType>(DerivedType *, QObject *)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QQmlFileSelector"
        ModifyFunction{
            signature: "setExtraSelectors(QStringList &)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setSelector(QFileSelector*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSelector"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    
    InterfaceType{
        name: "QQmlPropertyValueSource"
    }
    
    GlobalFunction{
        signature: "qmlRegisterCustomExtendedType<T,E>(const char*, int, int, const char*, QQmlCustomParser*)"
        remove: RemoveFlag.All
    }
    
    GlobalFunction{
        signature: "qmlRegisterCustomType<T>(const char*, int, int, const char*, QQmlCustomParser*)"
        remove: RemoveFlag.All
    }
    
    GlobalFunction{
        signature: "qjsEngine(const QObject*)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlAttachedPropertiesFunction(QObject*, const QMetaObject*)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlAttachedPropertiesObject(QObject*, QQmlAttachedPropertiesFunc, bool)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlClearTypeRegistrations()"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlContext(const QObject*)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlWarning(const QObject*)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlWarning(const QObject*, const QList<QQmlError>&)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlWarning(const QObject*, const QQmlError&)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlDebug(const QObject*)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlDebug(const QObject*, const QList<QQmlError>&)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlDebug(const QObject*, const QQmlError&)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlEngine(const QObject*)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlExecuteDeferred(QObject*)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlExtendedObject(QObject*)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlInfo(const QObject*)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlInfo(const QObject*, const QList<QQmlError>&)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlInfo(const QObject*, const QQmlError&)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlProtectModule(const char*, int)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlRegisterModule(const char*, int, int)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlRegisterModuleImport(const char*, int, const char*, int, int)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlRegisterNamespaceAndRevisions(const QMetaObject*, const char*, int)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlRegisterNamespaceAndRevisions(const QMetaObject*, const char*, int, QList<int>*, const QMetaObject*)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlRegisterNamespaceAndRevisions(const QMetaObject*, const char*, int, QList<int>*, const QMetaObject*, const QMetaObject*)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlRegisterSingletonType(QUrl, const char*, int, int, const char*)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlRegisterType(QUrl, const char*, int, int, const char*)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlRegisterTypeNotAvailable(const char*, int, int, const char*, QString)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlRegisterUncreatableMetaObject(const QMetaObject&, const char*, int, int, const char*, QString)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlTypeId(const char*, int, int, const char*)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlUnregisterModuleImport(const char*, int, const char*, int, int)"
        targetType: "QtQml"
    }
    
    GlobalFunction{
        signature: "qmlAttachedPropertiesObject<T>(const QObject*, bool)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlAttachedPropertiesObject"
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<? extends io.qt.core.QObject>"
            }
        }
    }
    
    GlobalFunction{
        signature: "qmlRegisterAnonymousSequentialContainer<Container>(const char*, int)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterAnonymousSequentialContainer"
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "containerType"
                type: "io.qt.core.QMetaType"
            }
        }
    }
    
    GlobalFunction{
        signature: "qmlRegisterAnonymousType<T>(const char*, int)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterAnonymousType"
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<?>"
            }
        }
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterAnonymousType"
            since: [6, 3]
            Argument{
                type: "QString"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<?>"
            }
            AddArgument{
                index: 2
                name: "metaObjectRevisionMinor"
                type: "int"
            }
        }
    }
    
    GlobalFunction{
        signature: "qmlRegisterType<T>()"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterType"
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<? extends io.qt.core.QObject>"
            }
        }
        until: [5, 15]
    }
    
    GlobalFunction{
        signature: "qmlRegisterExtendedType<T,E>()"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterExtendedType"
            Argument{
                type: "QObject"
            }
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<? extends io.qt.core.QObject>"
            }
            AddArgument{
                index: 2
                name: "extendedType"
                type: "java.lang.Class<? extends io.qt.core.QObject>"
            }
        }
        until: [5, 15]
    }
    
    GlobalFunction{
        signature: "qmlRegisterAnonymousTypesAndRevisions<T>(const char*, int)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterAnonymousTypesAndRevisions"
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<?>"
            }
        }
    }
    
    GlobalFunction{
        signature: "qmlRegisterInterface<T>(const char*, int)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterInterface"
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<? extends QtObjectInterface>"
            }
        }
    }
    
    GlobalFunction{
        signature: "qmlRegisterInterface<T>(const char*)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterInterface"
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<? extends QtObjectInterface>"
            }
        }
        until: [5, 15]
    }
    
    GlobalFunction{
        signature: "qmlRegisterRevision<T,metaObjectRevision>(const char*, int, int)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterRevision"
            Argument{
                type: "QObject"
            }
            Argument{
                type: "int"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<?>"
            }
            AddArgument{
                index: 2
                name: "metaObjectRevision"
                type: "int"
            }
        }
    }
    
    GlobalFunction{
        signature: "qmlRegisterExtendedType<T,E>(const char*, int)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterExtendedType"
            Argument{
                type: "QObject"
            }
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<?>"
            }
            AddArgument{
                index: 2
                name: "extendedType"
                type: "java.lang.Class<?>"
            }
        }
    }
    
    GlobalFunction{
        signature: "qmlRegisterExtendedType<T,E>(const char*, int, int, const char*)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterExtendedType"
            Argument{
                type: "QObject"
            }
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<?>"
            }
            AddArgument{
                index: 2
                name: "extendedType"
                type: "java.lang.Class<?>"
            }
        }
    }
    
    GlobalFunction{
        signature: "qmlRegisterExtendedUncreatableType<T,E>(const char*, int, int, const char*, QString)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterExtendedUncreatableType"
            Argument{
                type: "QObject"
            }
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<?>"
            }
            AddArgument{
                index: 2
                name: "extendedType"
                type: "java.lang.Class<?>"
            }
        }
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterExtendedUncreatableType"
            Argument{
                type: "QObject"
            }
            Argument{
                type: "QString"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<?>"
            }
            AddArgument{
                index: 2
                name: "extendedType"
                type: "java.lang.Class<?>"
            }
            AddArgument{
                index: 3
                name: "metaObjectRevision"
                type: "int"
            }
        }
    }
    
    GlobalFunction{
        signature: "qmlRegisterSingletonType<T>(const char*,int,int,const char*,QObject*(*)(QQmlEngine*,QJSEngine*))"
        targetType: "QtQml"
        ModifyArgument{
            index: 5
            ReplaceType{
                modifiedType: "io.qt.qml.QtQml$@Nullable ObjectCallback"
            }
            ConversionRule{
                codeClass: CodeClass.Native
                Text{content: "QtQml::ObjectCallback %out = qtjambi_cast<QtQml::ObjectCallback>(%env, %in);"}
            }
        }
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterSingletonType"
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<? extends io.qt.core.QObject>"
            }
        }
    }

    GlobalFunction{
        signature: "qmlRegisterSingletonInstance<T>(const char*, int, int, const char*, T*)"
        remove: RemoveFlag.All
    }
    
    GlobalFunction{
        signature: "qmlRegisterSingletonType(const char*,int,int,const char*,QJSValue(*)(QQmlEngine*,QJSEngine*))"
        proxyCall: "qtjambi_qmlRegisterSingletonType"
        targetType: "QtQml"
        ModifyArgument{
            index: 5
            ReplaceType{
                modifiedType: "io.qt.qml.QtQml$@Nullable ValueCallback"
            }
            ConversionRule{
                codeClass: CodeClass.Native
                Text{content: "QtQml::ValueCallback %out = qtjambi_cast<QtQml::ValueCallback>(%env, %in);"}
            }
        }
    }
    
    GlobalFunction{
        signature: "qmlRegisterType<T>(const char*, int, int, const char*)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterType"
            Argument{
                type: "QObject"
            }
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<?>"
            }
        }
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterType"
            Argument{
                type: "QString"
            }
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<?>"
            }
            AddArgument{
                index: 2
                name: "metaObjectRevision"
                type: "int"
            }
        }
    }
    
    GlobalFunction{
        signature: "qmlRegisterUncreatableType<T>(const char*, int, int, const char*, QString)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterUncreatableType"
            Argument{
                type: "QString"
            }
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<?>"
            }
        }
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterUncreatableType"
            Argument{
                type: "QObject"
            }
            Argument{
                type: "QObject"
            }
            AddArgument{
                index: 1
                name: "type"
                type: "java.lang.Class<?>"
            }
            AddArgument{
                index: 2
                name: "metaObjectRevision"
                type: "int"
            }
        }
    }
    
    GlobalFunction{
        signature: "qmlRegisterTypesAndRevisions<T,Args...>(const char*, int)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterTypesAndRevisions"
            Argument{
                type: "QObject"
            }
            Argument{
                type: "QObject"
            }
            Argument{
                type: "QObject"
            }
            AddArgument{
                name: "types"
                type: "java.lang.Class<?>..."
            }
        }
    }

    GlobalFunction{
        signature: "qmlRegisterTypesAndRevisions<>(const char *, int, QList<int> *)"
        remove: RemoveFlag.All
    }
    
    GlobalFunction{
        signature: "qmlRegisterTypesAndRevisions<T,Args...>(const char*, int, QList<int> *)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterTypesAndRevisions"
            Argument{
                type: "QObject"
            }
            Argument{
                type: "QObject"
            }
            Argument{
                type: "QObject"
            }
            AddArgument{
                name: "types"
                type: "java.lang.Class<?>..."
            }
        }
    }
    
    GlobalFunction{
        signature: "qmlRegisterTypesAndRevisions<Args...>(const char*, int, QList<int> *)"
        targetType: "QtQml"
        Instantiation{
            proxyCall: "qtjambi_qmlRegisterTypesAndRevisions"
            Argument{
                type: "QObject"
            }
            Argument{
                type: "QObject"
            }
            AddArgument{
                name: "types"
                type: "java.lang.Class<?>..."
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
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QQmlPropertyMap::QQmlPropertyMap', unmatched parameter type 'DerivedType*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *unmatched *type '*QMetaObject*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *unmatched *type '*QMetaMethod*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *unmatched *type '*QMetaProperty*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *unmatched *type '*QQmlComponentAttached*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *unmatched *type '*QQmlV8*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *unmatched *type '*QV8*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *unmatched *type '*QQmlCompiledData*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *unmatched *type '*QQmlAbstractUrlInterceptor*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *unmatched *type '*QQmlContextData*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QJSValue::QJSValue', unmatched parameter type 'QScriptPassPointer<QJSValuePrivate>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched * type 'QQmlV4Function*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched * type 'QV4::*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched * type 'QQmlInfo'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unknown operator 'T::QmlForeignType'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: enum 'QQmlModuleImportSpecialVersions' does not have a type entry or is not an enum"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QJSValue."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QQmlContext::PropertyPair."}
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: protected function '*' in final class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: visibility of function '*' modified in class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: hiding of function '*' in class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping field 'QQmlListProperty::*' with unmatched type 'QQmlListProperty::*'"}
}
