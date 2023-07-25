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
    packageName: "io.qt.statemachine"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtStateMachine"
    module: "qtjambi.statemachine"
    description: "Provides classes and tools for creating state machines from SCXML files and embedding them in applications."
    
    ObjectType{
        name: "QAbstractState"
        ModifyFunction{
            signature: "onEntry(QEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "onExit(QEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QAbstractTransition"

        EnumType{
            name: "TransitionType"
        }
        ModifyFunction{
            signature: "addAnimation(QAbstractAnimation*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcAnimation"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeAnimation(QAbstractAnimation*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcAnimation"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "eventTest(QEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "onTransition(QEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "addAnimation(QAbstractAnimation *)"
            ppCondition: "QT_CONFIG(animation)"
        }
        ModifyFunction{
            signature: "removeAnimation(QAbstractAnimation *)"
            ppCondition: "QT_CONFIG(animation)"
        }
        ModifyFunction{
            signature: "animations() const"
            ppCondition: "QT_CONFIG(animation)"
        }
        ModifyFunction{
            signature: "setTargetState(QAbstractState*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcTargetStates"
                    action: ReferenceCount.ClearAdd
                }
            }
        }
        ModifyFunction{
            signature: "setTargetStates(const QList<QAbstractState*>&)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcTargetStates"
                    action: ReferenceCount.ClearAddAll
                }
            }
        }
    }
    
    ObjectType{
        name: "QState"

        EnumType{
            name: "ChildMode"
        }

        EnumType{
            name: "RestorePolicy"
        }
        ModifyFunction{
            signature: "addTransition(QAbstractTransition*)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "removeTransition(QAbstractTransition*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setErrorState(QAbstractState*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcErrorState"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setInitialState(QAbstractState*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcInitialState"
                    action: ReferenceCount.Set
                }
            }
        }
        InjectCode{
            target: CodeClass.Java
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiCore.java"
                quoteAfterLine: "class QState___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "addTransition(const QObject *, const char *, QAbstractState *)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: 3
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 2
                    metaName: "signal"
                }
                ArgumentMap{
                    index: 1
                    metaName: "sender"
                }
                Text{content: "if(signal==null || signal.isEmpty())\n"+
                              "    return null;\n"+
                              "if(!signal.startsWith(\"2\")){\n"+
                              "    io.qt.core.QMetaMethod method = sender.metaObject().method(signal);\n"+
                              "    if(method!=null && method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal) {\n"+
                              "        signal = \"2\" + method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}"}
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{
                    index: 0
                    metaName: "%0"
                }
                ArgumentMap{
                    index: 3
                    metaName: "%3"
                }
                Text{content: "QtJambi_LibraryUtilities.internal.addReferenceCount(%0, QAbstractTransition.class, \"__rcTargetStates\", false, false, %3);"}
            }
        }
        ModifyFunction{
            signature: "addTransition(QAbstractState*)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{
                    index: 0
                    metaName: "%0"
                }
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "QtJambi_LibraryUtilities.internal.addReferenceCount(%0, QAbstractTransition.class, \"__rcTargetStates\", false, false, %1);"}
            }
        }
        InjectCode{
            target: CodeClass.Java
        }
    }
    
    ObjectType{
        name: "QStateMachine"

        EnumType{
            name: "Error"
        }

        EnumType{
            name: "EventPriority"
        }

        ObjectType{
            name: "SignalEvent"
        }

        ObjectType{
            name: "WrappedEvent"
        }
        ModifyFunction{
            signature: "beginMicrostep(QEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "beginSelectTransitions(QEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "endMicrostep(QEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "endSelectTransitions(QEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "addDefaultAnimation(QAbstractAnimation*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDefaultAnimations"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeDefaultAnimation(QAbstractAnimation*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDefaultAnimations"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "addState(QAbstractState*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcStates"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeState(QAbstractState*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcStates"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QHistoryState"
        EnumType{
            name: "HistoryType"
        }
        ModifyFunction{
            signature: "setDefaultTransition(QAbstractTransition*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setDefaultState(QAbstractState*)"
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
                    index: 0
                    metaName: "%0"
                }
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "QtJambi_LibraryUtilities.internal.addReferenceCount(defaultTransition(), QAbstractTransition.class, \"__rcTargetStates\", false, false, %1);"}
            }
        }
    }
    
    ObjectType{
        name: "QSignalTransition"
        ModifyFunction{
            signature: "setSenderObject(const QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSenderObject"
                    action: ReferenceCount.Set
                }
            }
        }
        InjectCode{
            target: CodeClass.Java
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiCore.java"
                quoteAfterLine: "class QSignalTransition___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "QSignalTransition(const QObject *, const char *, QState *)"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 2
                    metaName: "signal"
                }
                ArgumentMap{
                    index: 1
                    metaName: "sender"
                }
                Text{content: "if(signal!=null && !signal.startsWith(\"2\")){\n"+
                              "    io.qt.core.QMetaMethod method = sender.metaObject().method(signal);\n"+
                              "    if(method!=null && method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal) {\n"+
                              "        signal = \"2\" + method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "setSignal(const QByteArray &)"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "signal"
                }
                Text{content: "if(signal!=null && !signal.startsWith(\"2\")){\n"+
                              "    io.qt.core.QMetaMethod method = senderObject().metaObject().method(signal.toString());\n"+
                              "    if(method!=null && method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal) {\n"+
                              "        signal = new io.qt.core.QByteArray(\"2\");\n"+
                              "        signal.append(method.cppMethodSignature());\n"+
                              "    }\n"+
                              "}"}
            }
        }
    }
    
    ObjectType{
        name: "QKeyEventTransition"
    }
    
    ObjectType{
        name: "QMouseEventTransition"
    }
    
    ObjectType{
        name: "QEventTransition"
        ModifyFunction{
            signature: "setEventSource(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcEventSource"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QFinalState"
    }
}
