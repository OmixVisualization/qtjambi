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
    packageName: "io.qt.scxml"
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtScxml"
    module: "qtjambi.scxml"
    description: "Provides classes and tools for creating state machines from SCXML files and embedding them in applications."

    Template{
        name: "scxml.AtomicBoolean_to_bool_ptr"
        Text{content: "bool* %out = nullptr;\n"+
                      "if(%in){\n"+
                      "    bool b = Java::Runtime::AtomicBoolean.get(%env, %in);\n"+
                      "    JavaException::check(%env QTJAMBI_STACKTRACEINFO );\n"+
                      "    %out = new bool(b);\n"+
                      "    %scope.addFinalAction([%env,%in,%out](){\n"+
                      "            jboolean bval = *%out;\n"+
                      "            delete %out;\n"+
                      "            Java::Runtime::AtomicBoolean.set(%env, %in, bval);\n"+
                      "        });\n"+
                      "}"}
    }
    
    Template{
        name: "scxml.bool_ptr_to_AtomicBoolean"
        Text{content: "if(%in){\n"+
                      "    %out = Java::Runtime::AtomicBoolean::newInstance(%env, *%in);\n"+
                      "    %scope.addFinalAction([%env,%in,%out](){\n"+
                      "            *%in = Java::Runtime::AtomicBoolean.get(%env, %out);\n"+
                      "        });\n"+
                      "}"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        until: [5, 11]
        Text{content: "void initialize_meta_info_ForeachLoopBody();"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        until: [5, 11]
        Text{content: "initialize_meta_info_ForeachLoopBody();"}
    }
    
    Rejection{
        className: "QScxmlStateMachine"
        functionName: "onExit"
    }
    
    Rejection{
        className: "QScxmlStateMachine"
        functionName: "onEntry"
    }
    
    NamespaceType{
        name: "QScxmlExecutableContent"
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiScxml.java"
                quoteAfterLine: "class QScxmlExecutableContent___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ValueType{
        name: "QScxmlExecutableContent::AssignmentInfo"
    }
    
    ValueType{
        name: "QScxmlExecutableContent::EvaluatorInfo"
    }
    
    ValueType{
        name: "QScxmlExecutableContent::ForeachInfo"
    }
    
    ValueType{
        name: "QScxmlExecutableContent::InvokeInfo"
        ModifyField{
            name: "finalize"
            rename: "finalizeId"
        }
    }
    
    ValueType{
        name: "QScxmlExecutableContent::ParameterInfo"
    }
    
    ValueType{
        name: "QScxmlError"
        ModifyFunction{
            signature: "operator=( const QScxmlError & )"
            remove: RemoveFlag.All
        }
    }
    
    InterfaceType{
        name: "QScxmlCompiler::Loader"
    }
    
    InterfaceType{
        name: "QScxmlTableData"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "stateMachineTable() const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.nio.IntBuffer"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "int* %out = reinterpret_cast<int*>(%env->GetDirectBufferAddress(%in));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = %env->NewDirectByteBuffer(const_cast<int*>(%in), 1024*1024 /* this number is an assumption */);\n"+
                                  "%out = Java::Runtime::ByteBuffer::asReadOnlyBuffer(%env,%out);\n"+
                                  "%out = Java::Runtime::ByteBuffer::asIntBuffer(%env,%out);"}
                }
            }
        }
        ModifyFunction{
            signature: "instructions() const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.nio.IntBuffer"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "int* %out = reinterpret_cast<int*>(%env->GetDirectBufferAddress(%in));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = %env->NewDirectByteBuffer(const_cast<int*>(%in), 1024*1024 /* this number is an assumption */);\n"+
                                  "%out = Java::Runtime::ByteBuffer::asReadOnlyBuffer(%env,%out);\n"+
                                  "%out = Java::Runtime::ByteBuffer::asIntBuffer(%env,%out);"}
                }
            }
        }
        ModifyFunction{
            signature: "dataNames(int *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.nio.IntBuffer"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "int* %out = reinterpret_cast<int*>(%env->GetDirectBufferAddress(%in));\n"+
                                  "if(%1)*%1 = qMax(0, int(%env->GetDirectBufferCapacity(%in)));"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = %env->NewDirectByteBuffer(%in, jlong(sizeof(int)) * count);\n"+
                                  "%out = Java::Runtime::ByteBuffer::asIntBuffer(%env,%out);"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "//convert shell"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int count = 0;\n"+
                                  "int* %out = &count;"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QScxmlCompiler"
        ModifyFunction{
            signature: "setLoader(QScxmlCompiler::Loader*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcLoader"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QScxmlCppDataModel"
        ModifyFunction{
            signature: "setScxmlEvent(const QScxmlEvent & )"
        }
    }
    
    ObjectType{
        name: "QScxmlDataModel"
        ModifyFunction{
            signature: "setStateMachine(QScxmlStateMachine*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcStateMachine"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "createScxmlDataModel(QString)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            since: [6, 1]
        }
        ModifyFunction{
            signature: "evaluateToString(int, bool *)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "boolean[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "BoolPointerArray %inArray(%env, %in, %in ? 1 : 0);\n"+
                                  "jbooleanArray %out = %inArray.array();"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JBooleanArrayPointer %out(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "evaluateToBool(int, bool *)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "boolean[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "BoolPointerArray %inArray(%env, %in, %in ? 1 : 0);\n"+
                                  "jbooleanArray %out = %inArray.array();"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JBooleanArrayPointer %out(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "evaluateToVariant(int, bool *)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "boolean[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "BoolPointerArray %inArray(%env, %in, %in ? 1 : 0);\n"+
                                  "jbooleanArray %out = %inArray.array();"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JBooleanArrayPointer %out(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "evaluateToVoid(int, bool *)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "boolean[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "BoolPointerArray %inArray(%env, %in, %in ? 1 : 0);\n"+
                                  "jbooleanArray %out = %inArray.array();"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JBooleanArrayPointer %out(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "evaluateAssignment(int, bool *)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "boolean[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "BoolPointerArray %inArray(%env, %in, %in ? 1 : 0);\n"+
                                  "jbooleanArray %out = %inArray.array();"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JBooleanArrayPointer %out(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "evaluateInitialization(int, bool *)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "boolean[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "BoolPointerArray %inArray(%env, %in, %in ? 1 : 0);\n"+
                                  "jbooleanArray %out = %inArray.array();"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JBooleanArrayPointer %out(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "evaluateForeach(int,bool*,QScxmlDataModel::ForeachLoopBody*)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "boolean[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "BoolPointerArray %inArray(%env, %in, %in ? 1 : 0);\n"+
                                  "jbooleanArray %out = %inArray.array();"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JBooleanArrayPointer %out(%env, %in);"}
                }
            }
            ModifyArgument{
                index: 3
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QScxmlDynamicScxmlServiceFactory"
    }
    
    ObjectType{
        name: "QScxmlEcmaScriptDataModel"
    }
    
    ValueType{
        name: "QScxmlEvent"
        ModifyFunction{
            signature: "operator=( const QScxmlEvent & )"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QScxmlInvokableService"
        ModifyFunction{
            signature: "postEvent(QScxmlEvent *)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.scxml.QScxmlEvent"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QScxmlEvent *%out = qtjambi_cast<QScxmlEvent *>(%env, %in);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QScxmlInvokableServiceFactory"
    }
    
    ObjectType{
        name: "QScxmlNullDataModel"
    }
    
    ObjectType{
        name: "QScxmlStateMachine"
        ModifyFunction{
            signature: "setDataModel(QScxmlDataModel*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDataModel"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setLoader(QScxmlCompiler::Loader*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcLoader"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setTableData(QScxmlTableData*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcTableData"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "connectToEvent(QString,const QObject*,const char*,Qt::ConnectionType)"
            access: Modification.Private
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "int"
                }
                RemoveDefaultExpression{
                }
            }
        }
        ModifyFunction{
            signature: "connectToState(QString,const QObject*,const char*,Qt::ConnectionType)"
            access: Modification.Private
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "int"
                }
                RemoveDefaultExpression{
                }
            }
        }
        ModifyFunction{
            signature: "submitEvent(QScxmlEvent *)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.scxml.QScxmlEvent"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QScxmlEvent *%out = qtjambi_cast<QScxmlEvent *>(%env, %in);"}
                }
            }
        }
        InjectCode{
            target: CodeClass.Java
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiScxml.java"
                quoteAfterLine: "class QScxmlStateMachine___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QScxmlStaticScxmlServiceFactory"
    }
    
    ObjectType{
        name: "QScxmlDataModel::ForeachLoopBody"
        forceAbstract: true
        generate: false
        ModifyFunction{
            signature: "ForeachLoopBody()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "run(bool*)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "boolean[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JBooleanArrayPointer %out(%env, %in);"}
                }
            }
        }
        until: [5, 11]
    }
    
    InterfaceType{
        name: "QScxmlDataModel::ForeachLoopBody"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "run(bool*)"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "boolean"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jboolean %out = %1;"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "if(%1){\n"+
                                  "    *%1 = %in;\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool %in = false;\n"+
                                  "bool* %out = &%in;"}
                }
            }
        }
        since: [5, 12]
    }
    
    EnumType{
        name: "QScxmlEvent::EventType"
    }
    
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QScxml*."}
}
