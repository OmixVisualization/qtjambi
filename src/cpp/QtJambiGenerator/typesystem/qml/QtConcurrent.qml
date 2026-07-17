/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    packageName: "io.qt.concurrent"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtConcurrent"
    module: "qtjambi.concurrent"
    description: "Classes for writing multi-threaded programs without using low-level threading primitives."

    TypeTemplate{
        name: "1Generics"
        isGeneric: true
        GenericArgument{name: "A"; comment: "first argument"}
    }
    TypeTemplate{
        name: "2Generics"
        Import{template: "1Generics"}
        GenericArgument{name: "B"; comment: "second argument"}
    }
    TypeTemplate{
        name: "3Generics"
        Import{template: "2Generics"}
        GenericArgument{name: "C"; comment: "third argument"}
    }
    TypeTemplate{
        name: "4Generics"
        Import{template: "3Generics"}
        GenericArgument{name: "D"; comment: "forth argument"}
    }
    TypeTemplate{
        name: "5Generics"
        Import{template: "4Generics"}
        GenericArgument{name: "E"; comment: "fifth argument"}
    }
    TypeTemplate{
        name: "6Generics"
        Import{template: "5Generics"}
        GenericArgument{name: "F"; comment: "sixth argument"}
    }
    TypeTemplate{
        name: "7Generics"
        Import{template: "6Generics"}
        GenericArgument{name: "G"; comment: "seventh argument"}
    }
    TypeTemplate{
        name: "8Generics"
        Import{template: "7Generics"}
        GenericArgument{name: "H"; comment: "eighth argument"}
    }
    TypeTemplate{
        name: "9Generics"
        Import{template: "8Generics"}
        GenericArgument{name: "I"; comment: "ninth argument"}
    }
    TypeTemplate{
        name: "GenericsT"
        isGeneric: true
        GenericArgument{name: "T"; comment: "return argument"}
    }

    TypeTemplate{
        name: "TaskBuilderTemplate"
        generate: "no-shell"
        forceFriendly: true
        forceFinal: true
        ModifyFunction{
            signature: "onThreadPool(QThreadPool&)"
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "withPriority(int)"
            ModifyArgument{
                index: 0
                replaceValue: "this"
            }
        }
        ModifyFunction{
            signature: "${typename}(QtConcurrent::QTaskBuilder<Task,Args>)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "${typename}(${typename})"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QTaskBuilder(${typename})"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "${typename}(Task &&,Args &&)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "${typename}(TaskStartParameters, Task &&,Args &&)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QTaskBuilder(Task &&,Args &&)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QTaskBuilder(TaskStartParameters, Task &&,Args &&)"
            remove: RemoveFlag.All
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateFuture"
        Import{template: "TaskBuilderTemplate"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/FutureCast"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/Template1Cast"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            remove: RemoveFlag.All
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateVoid"
        defaultSuperClass: "QTaskBuilder<@QtPrimitiveType Void>"
        Import{template: "TaskBuilderTemplateFuture"}
        ModifyFunction{
            signature: "spawn()"
            ModifyArgument{ index: 0; replaceType: "io.qt.core.QFuture<@QtPrimitiveType Void>" }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplatePromise"
        generate: "no-shell"
        ModifyFunction{
            signature: "spawn(QtConcurrent::FutureResult)"
            remove: RemoveFlag.All
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplatePromiseVoid"
        Import{template: "TaskBuilderTemplateVoid"}
        Import{template: "TaskBuilderTemplatePromise"}
    }
    TypeTemplate{
        name: "TaskBuilderTemplateTypedReturn"
        defaultSuperClass: "QTaskBuilder<T>"
        Import{template: "GenericsT"}
        Import{template: "TaskBuilderTemplateFuture"}
        ModifyFunction{
            signature: "spawn()"
            ModifyArgument{ index: 0; replaceType: "io.qt.core.QFuture<T>" }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateTypedPromise"
        Import{template: "TaskBuilderTemplateTypedReturn"}
        Import{template: "TaskBuilderTemplatePromise"}
    }
    TypeTemplate{
        name: "TaskBuilderTemplatePlainVoid"
        defaultSuperClass: "QTaskBuilder<Object>"
        Import{template: "TaskBuilderTemplateFuture"}
        ModifyFunction{
            signature: "spawn()"
            ModifyArgument{ index: 0; replaceType: "io.qt.core.QFuture<Object>" }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplatePlainTypedReturn"
        Import{template: "TaskBuilderTemplatePlainVoid"}
    }
    TypeTemplate{
        name: "TaskBuilderTemplatePlainPromiseVoid"
        Import{template: "TaskBuilderTemplatePlainTypedReturn"}
        Import{template: "TaskBuilderTemplatePromise"}
    }
    TypeTemplate{
        name: "TaskBuilderTemplatePlainTypedPromise"
        Import{template: "TaskBuilderTemplatePlainPromiseVoid"}
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments"
        Import{template: "TaskBuilderTemplate"}
        ModifyFunction{
            signature: "spawn(QtConcurrent::FutureResult)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "spawn()"
            remove: RemoveFlag.All
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments1V"
        defaultSuperClass: "QTaskBuilder.Params1<@QtPrimitiveType Void,A>"
        Import{template: "1Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template2Cast"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<@QtPrimitiveType Void>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    Text{content: "testArguments(%1);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments2V"
        defaultSuperClass: "QTaskBuilder.Params2<@QtPrimitiveType Void,A,B>"
        Import{template: "2Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template3Cast"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<@QtPrimitiveType Void>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    Text{content: "testArguments(%1, %2);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments3V"
        defaultSuperClass: "QTaskBuilder.Params3<@QtPrimitiveType Void,A,B,C>"
        Import{template: "3Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template4Cast"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 3; replaceType: "C"; rename: "c" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<@QtPrimitiveType Void>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    Text{content: "testArguments(%1, %2, %3);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments4V"
        defaultSuperClass: "QTaskBuilder.Params4<@QtPrimitiveType Void,A,B,C,D>"
        Import{template: "4Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template5Cast"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 3; replaceType: "C"; rename: "c" }
                ModifyArgument{ index: 4; replaceType: "D"; rename: "d" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<@QtPrimitiveType Void>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    Text{content: "testArguments(%1, %2, %3, %4);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments5V"
        defaultSuperClass: "QTaskBuilder.Params5<@QtPrimitiveType Void,A,B,C,D,E>"
        Import{template: "5Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 3; replaceType: "C"; rename: "c" }
                ModifyArgument{ index: 4; replaceType: "D"; rename: "d" }
                ModifyArgument{ index: 5; replaceType: "E"; rename: "e" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<@QtPrimitiveType Void>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments6V"
        defaultSuperClass: "QTaskBuilder.Params6<@QtPrimitiveType Void,A,B,C,D,E,F>"
        Import{template: "6Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 3; replaceType: "C"; rename: "c" }
                ModifyArgument{ index: 4; replaceType: "D"; rename: "d" }
                ModifyArgument{ index: 5; replaceType: "E"; rename: "e" }
                ModifyArgument{ index: 6; replaceType: "F"; rename: "f" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<@QtPrimitiveType Void>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    ArgumentMap{index: 6; metaName: "%6"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5, %6);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments7V"
        defaultSuperClass: "QTaskBuilder.Params7<@QtPrimitiveType Void,A,B,C,D,E,F,G>"
        Import{template: "7Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 3; replaceType: "C"; rename: "c" }
                ModifyArgument{ index: 4; replaceType: "D"; rename: "d" }
                ModifyArgument{ index: 5; replaceType: "E"; rename: "e" }
                ModifyArgument{ index: 6; replaceType: "F"; rename: "f" }
                ModifyArgument{ index: 7; replaceType: "G"; rename: "g" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<@QtPrimitiveType Void>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    ArgumentMap{index: 6; metaName: "%6"}
                    ArgumentMap{index: 7; metaName: "%7"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5, %6, %7);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments8V"
        defaultSuperClass: "QTaskBuilder.Params8<@QtPrimitiveType Void,A,B,C,D,E,F,G,H>"
        Import{template: "8Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 3; replaceType: "C"; rename: "c" }
                ModifyArgument{ index: 4; replaceType: "D"; rename: "d" }
                ModifyArgument{ index: 5; replaceType: "E"; rename: "e" }
                ModifyArgument{ index: 6; replaceType: "F"; rename: "f" }
                ModifyArgument{ index: 7; replaceType: "G"; rename: "g" }
                ModifyArgument{ index: 8; replaceType: "H"; rename: "h" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<@QtPrimitiveType Void>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    ArgumentMap{index: 6; metaName: "%6"}
                    ArgumentMap{index: 7; metaName: "%7"}
                    ArgumentMap{index: 8; metaName: "%8"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5, %6, %7, %8);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments9V"
        defaultSuperClass: "QTaskBuilder.Params9<@QtPrimitiveType Void,A,B,C,D,E,F,G,H,I>"
        Import{template: "9Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 3; replaceType: "C"; rename: "c" }
                ModifyArgument{ index: 4; replaceType: "D"; rename: "d" }
                ModifyArgument{ index: 5; replaceType: "E"; rename: "e" }
                ModifyArgument{ index: 6; replaceType: "F"; rename: "f" }
                ModifyArgument{ index: 7; replaceType: "G"; rename: "g" }
                ModifyArgument{ index: 8; replaceType: "H"; rename: "h" }
                ModifyArgument{ index: 9; replaceType: "I"; rename: "i" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<@QtPrimitiveType Void>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    ArgumentMap{index: 6; metaName: "%6"}
                    ArgumentMap{index: 7; metaName: "%7"}
                    ArgumentMap{index: 8; metaName: "%8"}
                    ArgumentMap{index: 9; metaName: "%9"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5, %6, %7, %8, %9);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments1T"
        defaultSuperClass: "QTaskBuilder.Params1<T,A>"
        Import{template: "1Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        Import{template: "GenericsT"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template2Cast"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<T>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    Text{content: "testArguments(%1);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments2T"
        defaultSuperClass: "QTaskBuilder.Params2<T,A,B>"
        Import{template: "2Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        Import{template: "GenericsT"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template3Cast"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<T>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    Text{content: "testArguments(%1, %2);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments3T"
        defaultSuperClass: "QTaskBuilder.Params3<T,A,B,C>"
        Import{template: "3Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        Import{template: "GenericsT"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template4Cast"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 3; replaceType: "C"; rename: "c" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<T>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    Text{content: "testArguments(%1, %2, %3);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments4T"
        defaultSuperClass: "QTaskBuilder.Params4<T,A,B,C,D>"
        Import{template: "4Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        Import{template: "GenericsT"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template5Cast"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 3; replaceType: "C"; rename: "c" }
                ModifyArgument{ index: 4; replaceType: "D"; rename: "d" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<T>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    Text{content: "testArguments(%1, %2, %3, %4);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments5T"
        defaultSuperClass: "QTaskBuilder.Params5<T,A,B,C,D,E>"
        Import{template: "5Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        Import{template: "GenericsT"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 3; replaceType: "C"; rename: "c" }
                ModifyArgument{ index: 4; replaceType: "D"; rename: "d" }
                ModifyArgument{ index: 5; replaceType: "E"; rename: "e" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<T>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments6T"
        defaultSuperClass: "QTaskBuilder.Params6<T,A,B,C,D,E,F>"
        Import{template: "6Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        Import{template: "GenericsT"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 3; replaceType: "C"; rename: "c" }
                ModifyArgument{ index: 4; replaceType: "D"; rename: "d" }
                ModifyArgument{ index: 5; replaceType: "E"; rename: "e" }
                ModifyArgument{ index: 6; replaceType: "F"; rename: "f" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<T>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    ArgumentMap{index: 6; metaName: "%6"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5, %6);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments7T"
        defaultSuperClass: "QTaskBuilder.Params7<T,A,B,C,D,E,F,G>"
        Import{template: "7Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        Import{template: "GenericsT"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 3; replaceType: "C"; rename: "c" }
                ModifyArgument{ index: 4; replaceType: "D"; rename: "d" }
                ModifyArgument{ index: 5; replaceType: "E"; rename: "e" }
                ModifyArgument{ index: 6; replaceType: "F"; rename: "f" }
                ModifyArgument{ index: 7; replaceType: "G"; rename: "g" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<T>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    ArgumentMap{index: 6; metaName: "%6"}
                    ArgumentMap{index: 7; metaName: "%7"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5, %6, %7);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments8T"
        defaultSuperClass: "QTaskBuilder.Params8<T,A,B,C,D,E,F,G,H>"
        Import{template: "8Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        Import{template: "GenericsT"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 3; replaceType: "C"; rename: "c" }
                ModifyArgument{ index: 4; replaceType: "D"; rename: "d" }
                ModifyArgument{ index: 5; replaceType: "E"; rename: "e" }
                ModifyArgument{ index: 6; replaceType: "F"; rename: "f" }
                ModifyArgument{ index: 7; replaceType: "G"; rename: "g" }
                ModifyArgument{ index: 8; replaceType: "H"; rename: "h" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<T>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    ArgumentMap{index: 6; metaName: "%6"}
                    ArgumentMap{index: 7; metaName: "%7"}
                    ArgumentMap{index: 8; metaName: "%8"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5, %6, %7, %8);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments9T"
        defaultSuperClass: "QTaskBuilder.Params9<T,A,B,C,D,E,F,G,H,I>"
        Import{template: "9Generics"}
        Import{template: "TaskBuilderTemplateArguments"}
        Import{template: "GenericsT"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; replaceType: "A"; rename: "a" }
                ModifyArgument{ index: 2; replaceType: "B"; rename: "b" }
                ModifyArgument{ index: 3; replaceType: "C"; rename: "c" }
                ModifyArgument{ index: 4; replaceType: "D"; rename: "d" }
                ModifyArgument{ index: 5; replaceType: "E"; rename: "e" }
                ModifyArgument{ index: 6; replaceType: "F"; rename: "f" }
                ModifyArgument{ index: 7; replaceType: "G"; rename: "g" }
                ModifyArgument{ index: 8; replaceType: "H"; rename: "h" }
                ModifyArgument{ index: 9; replaceType: "I"; rename: "i" }
                ModifyArgument{ index: 0; replaceType: "@NonNull QTaskBuilder<T>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    ArgumentMap{index: 6; metaName: "%6"}
                    ArgumentMap{index: 7; metaName: "%7"}
                    ArgumentMap{index: 8; metaName: "%8"}
                    ArgumentMap{index: 9; metaName: "%9"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5, %6, %7, %8, %9);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments1PlainV"
        defaultSuperClass: "QTaskBuilder.Params1<Object,Object>"
        Import{template: "TaskBuilderTemplateArguments"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template2Cast"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; rename: "a" }
                ModifyArgument{ index: 0; replaceType: "QTaskBuilder<Object>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    Text{content: "testArguments(%1);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments2PlainV"
        defaultSuperClass: "QTaskBuilder.Params2<Object,Object,Object>"
        Import{template: "TaskBuilderTemplateArguments"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template3Cast"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; rename: "a" }
                ModifyArgument{ index: 2; rename: "b" }
                ModifyArgument{ index: 0; replaceType: "QTaskBuilder<Object>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    Text{content: "testArguments(%1, %2);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments3PlainV"
        defaultSuperClass: "QTaskBuilder.Params3<Object,Object,Object,Object>"
        Import{template: "TaskBuilderTemplateArguments"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template4Cast"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; rename: "a" }
                ModifyArgument{ index: 2; rename: "b" }
                ModifyArgument{ index: 3; rename: "c" }
                ModifyArgument{ index: 0; replaceType: "QTaskBuilder<Object>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    Text{content: "testArguments(%1, %2, %3);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments4PlainV"
        defaultSuperClass: "QTaskBuilder.Params4<Object,Object,Object,Object,Object>"
        Import{template: "TaskBuilderTemplateArguments"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template5Cast"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; rename: "a" }
                ModifyArgument{ index: 2; rename: "b" }
                ModifyArgument{ index: 3; rename: "c" }
                ModifyArgument{ index: 4; rename: "d" }
                ModifyArgument{ index: 0; replaceType: "QTaskBuilder<Object>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    Text{content: "testArguments(%1, %2, %3, %4);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments5PlainV"
        defaultSuperClass: "QTaskBuilder.Params5<Object,Object,Object,Object,Object,Object>"
        Import{template: "TaskBuilderTemplateArguments"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; rename: "a" }
                ModifyArgument{ index: 2; rename: "b" }
                ModifyArgument{ index: 3; rename: "c" }
                ModifyArgument{ index: 4; rename: "d" }
                ModifyArgument{ index: 5; rename: "e" }
                ModifyArgument{ index: 0; replaceType: "QTaskBuilder<Object>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments6PlainV"
        defaultSuperClass: "QTaskBuilder.Params6<Object,Object,Object,Object,Object,Object,Object>"
        Import{template: "TaskBuilderTemplateArguments"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; rename: "a" }
                ModifyArgument{ index: 2; rename: "b" }
                ModifyArgument{ index: 3; rename: "c" }
                ModifyArgument{ index: 4; rename: "d" }
                ModifyArgument{ index: 5; rename: "e" }
                ModifyArgument{ index: 6; rename: "f" }
                ModifyArgument{ index: 0; replaceType: "QTaskBuilder<Object>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    ArgumentMap{index: 6; metaName: "%6"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5, %6);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments7PlainV"
        defaultSuperClass: "QTaskBuilder.Params7<Object,Object,Object,Object,Object,Object,Object,Object>"
        Import{template: "TaskBuilderTemplateArguments"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; rename: "a" }
                ModifyArgument{ index: 2; rename: "b" }
                ModifyArgument{ index: 3; rename: "c" }
                ModifyArgument{ index: 4; rename: "d" }
                ModifyArgument{ index: 5; rename: "e" }
                ModifyArgument{ index: 6; rename: "f" }
                ModifyArgument{ index: 7; rename: "g" }
                ModifyArgument{ index: 0; replaceType: "QTaskBuilder<Object>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    ArgumentMap{index: 6; metaName: "%6"}
                    ArgumentMap{index: 7; metaName: "%7"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5, %6, %7);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments8PlainV"
        defaultSuperClass: "QTaskBuilder.Params8<Object,Object,Object,Object,Object,Object,Object,Object,Object>"
        Import{template: "TaskBuilderTemplateArguments"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; rename: "a" }
                ModifyArgument{ index: 2; rename: "b" }
                ModifyArgument{ index: 3; rename: "c" }
                ModifyArgument{ index: 4; rename: "d" }
                ModifyArgument{ index: 5; rename: "e" }
                ModifyArgument{ index: 6; rename: "f" }
                ModifyArgument{ index: 7; rename: "g" }
                ModifyArgument{ index: 8; rename: "h" }
                ModifyArgument{ index: 0; replaceType: "QTaskBuilder<Object>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    ArgumentMap{index: 6; metaName: "%6"}
                    ArgumentMap{index: 7; metaName: "%7"}
                    ArgumentMap{index: 8; metaName: "%8"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5, %6, %7, %8);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments9PlainV"
        defaultSuperClass: "QTaskBuilder.Params9<Object,Object,Object,Object,Object,Object,Object,Object,Object,Object>"
        Import{template: "TaskBuilderTemplateArguments"}
        ModifyFunction{
            signature: "withArguments<ExtraArgs...>(ExtraArgs&&)"
            Instantiation{
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                Argument{ type: "JObjectWrapper"; isImplicit: true }
                ModifyArgument{ index: 1; rename: "a" }
                ModifyArgument{ index: 2; rename: "b" }
                ModifyArgument{ index: 3; rename: "c" }
                ModifyArgument{ index: 4; rename: "d" }
                ModifyArgument{ index: 5; rename: "e" }
                ModifyArgument{ index: 6; rename: "f" }
                ModifyArgument{ index: 7; rename: "g" }
                ModifyArgument{ index: 8; rename: "h" }
                ModifyArgument{ index: 9; rename: "i" }
                ModifyArgument{ index: 0; replaceType: "QTaskBuilder<Object>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 2; metaName: "%2"}
                    ArgumentMap{index: 3; metaName: "%3"}
                    ArgumentMap{index: 4; metaName: "%4"}
                    ArgumentMap{index: 5; metaName: "%5"}
                    ArgumentMap{index: 6; metaName: "%6"}
                    ArgumentMap{index: 7; metaName: "%7"}
                    ArgumentMap{index: 8; metaName: "%8"}
                    ArgumentMap{index: 9; metaName: "%9"}
                    Text{content: "testArguments(%1, %2, %3, %4, %5, %6, %7, %8, %9);"}
                }
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments1PlainT"
        Import{template: "TaskBuilderTemplateArguments1PlainV"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template2Cast"
                location: Include.Global
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments2PlainT"
        Import{template: "TaskBuilderTemplateArguments2PlainV"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template3Cast"
                location: Include.Global
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments3PlainT"
        Import{template: "TaskBuilderTemplateArguments3PlainV"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template4Cast"
                location: Include.Global
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments4PlainT"
        Import{template: "TaskBuilderTemplateArguments4PlainV"}
        ExtraIncludes{
            Include{
                fileName: "QtJambi/Template5Cast"
                location: Include.Global
            }
        }
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments5PlainT"
        Import{template: "TaskBuilderTemplateArguments5PlainV"}
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments6PlainT"
        Import{template: "TaskBuilderTemplateArguments6PlainV"}
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments7PlainT"
        Import{template: "TaskBuilderTemplateArguments7PlainV"}
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments8PlainT"
        Import{template: "TaskBuilderTemplateArguments8PlainV"}
    }
    TypeTemplate{
        name: "TaskBuilderTemplateArguments9PlainT"
        Import{template: "TaskBuilderTemplateArguments9PlainV"}
    }

    NamespaceType{
        name: "QtConcurrent"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        InjectCode{
            target: CodeClass.MetaInfo
            Text{content: String.raw`
                {
                    const std::type_info& typeId = registerValueTypeInfo<QtConcurrent::ThreadEngineStarterWrapper>("QtConcurrent::ThreadEngineStarterWrapper", "io/qt/concurrent/QtConcurrent$ThreadEngineStarter");
                    registerMetaType<QtConcurrent::ThreadEngineStarterWrapper>("QtConcurrent::ThreadEngineStarterWrapper");
                    registerDeleter(typeId, &QtConcurrent::ThreadEngineStarterWrapper::destroy);
                }`}
        }
        Rejection{enumName: "enum_1"}
        Rejection{functionName: "operator|"}
        Rejection{functionName: "filterInternal"}
        Rejection{
            className: "BlockSizeManager"
        }

        Rejection{
            className: "BlockSizeManagerV2"
        }

        Rejection{
            className: "ConstMemberFunctionWrapper"
        }

        Rejection{
            className: "Exception"
        }

        Rejection{
            className: "FilterKernel"
        }

        Rejection{
            className: "FilteredEachKernel"
        }

        Rejection{
            className: "FilteredReducedKernel"
        }

        Rejection{
            className: "FunctionWrapper0"
        }

        Rejection{
            className: "FunctionWrapper1"
        }

        Rejection{
            className: "FunctionWrapper2"
        }

        Rejection{
            className: "IntermediateResults"
        }

        Rejection{
            className: "DefaultValueContainer"
        }

        Rejection{
            className: "IterateKernel"
        }

        Rejection{
            className: "MapKernel"
        }

        Rejection{
            className: "MappedEachKernel"
        }

        Rejection{
            className: "MappedReducedKernel"
        }

        Rejection{
            className: "Median"
        }

        Rejection{
            className: "MemberFunctionWrapper"
        }

        Rejection{
            className: "MemberFunctionWrapper1"
        }

        Rejection{
            className: "qValueType"
        }

        Rejection{
            className: "ReduceKernel"
        }

        Rejection{
            className: "ResultItem"
        }

        Rejection{
            className: "ResultIterator"
        }

        Rejection{
            className: "ResultIteratorBase"
        }

        Rejection{
            className: "ResultReporter"
        }

        Rejection{
            className: "ResultStore"
        }

        Rejection{
            className: "ResultStoreBase"
        }

        Rejection{
            className: "RunFunctionTask"
        }

        Rejection{
            className: "RunFunctionTaskBase"
        }

        Rejection{
            className: "SelectSpecialization"
        }

        Rejection{
            className: "SelectStoredConstMemberFunctionCall0"
        }

        Rejection{
            className: "SelectStoredConstMemberFunctionCall1"
        }

        Rejection{
            className: "SelectStoredConstMemberFunctionCall2"
        }

        Rejection{
            className: "SelectStoredConstMemberFunctionCall3"
        }

        Rejection{
            className: "SelectStoredConstMemberFunctionCall4"
        }

        Rejection{
            className: "SelectStoredConstMemberFunctionCall5"
        }

        Rejection{
            className: "SelectStoredConstMemberFunctionPointerCall0"
        }

        Rejection{
            className: "SelectStoredConstMemberFunctionPointerCall1"
        }

        Rejection{
            className: "SelectStoredConstMemberFunctionPointerCall2"
        }

        Rejection{
            className: "SelectStoredConstMemberFunctionPointerCall3"
        }

        Rejection{
            className: "SelectStoredConstMemberFunctionPointerCall4"
        }

        Rejection{
            className: "SelectStoredConstMemberFunctionPointerCall5"
        }

        Rejection{
            className: "SelectStoredFunctorCall0"
        }

        Rejection{
            className: "SelectStoredFunctorCall1"
        }

        Rejection{
            className: "SelectStoredFunctorCall2"
        }

        Rejection{
            className: "SelectStoredFunctorCall3"
        }

        Rejection{
            className: "SelectStoredFunctorCall4"
        }

        Rejection{
            className: "SelectStoredFunctorCall5"
        }

        Rejection{
            className: "SelectStoredFunctorPointerCall0"
        }

        Rejection{
            className: "SelectStoredFunctorPointerCall1"
        }

        Rejection{
            className: "SelectStoredFunctorPointerCall2"
        }

        Rejection{
            className: "SelectStoredFunctorPointerCall3"
        }

        Rejection{
            className: "SelectStoredFunctorPointerCall4"
        }

        Rejection{
            className: "SelectStoredFunctorPointerCall5"
        }

        Rejection{
            className: "SelectStoredMemberFunctionCall0"
        }

        Rejection{
            className: "SelectStoredMemberFunctionCall1"
        }

        Rejection{
            className: "SelectStoredMemberFunctionCall2"
        }

        Rejection{
            className: "SelectStoredMemberFunctionCall3"
        }

        Rejection{
            className: "SelectStoredMemberFunctionCall4"
        }

        Rejection{
            className: "SelectStoredMemberFunctionCall5"
        }

        Rejection{
            className: "SelectStoredMemberFunctionPointerCall0"
        }

        Rejection{
            className: "SelectStoredMemberFunctionPointerCall1"
        }

        Rejection{
            className: "SelectStoredMemberFunctionPointerCall2"
        }

        Rejection{
            className: "SelectStoredMemberFunctionPointerCall3"
        }

        Rejection{
            className: "SelectStoredMemberFunctionPointerCall4"
        }

        Rejection{
            className: "SelectStoredMemberFunctionPointerCall5"
        }

        Rejection{
            className: "SequenceHolder1"
        }

        Rejection{
            className: "SequenceHolder2"
        }

        Rejection{
            className: "StoredConstMemberFunctionCall0"
        }

        Rejection{
            className: "StoredConstMemberFunctionCall1"
        }

        Rejection{
            className: "StoredConstMemberFunctionCall2"
        }

        Rejection{
            className: "StoredConstMemberFunctionCall3"
        }

        Rejection{
            className: "StoredConstMemberFunctionCall4"
        }

        Rejection{
            className: "StoredConstMemberFunctionCall5"
        }

        Rejection{
            className: "StoredConstMemberFunctionPointerCall0"
        }

        Rejection{
            className: "StoredConstMemberFunctionPointerCall1"
        }

        Rejection{
            className: "StoredConstMemberFunctionPointerCall2"
        }

        Rejection{
            className: "StoredConstMemberFunctionPointerCall3"
        }

        Rejection{
            className: "StoredConstMemberFunctionPointerCall4"
        }

        Rejection{
            className: "StoredConstMemberFunctionPointerCall5"
        }

        Rejection{
            className: "StoredFunctorCall0"
        }

        Rejection{
            className: "StoredFunctorCall1"
        }

        Rejection{
            className: "StoredFunctorCall2"
        }

        Rejection{
            className: "StoredFunctorCall3"
        }

        Rejection{
            className: "StoredFunctorCall4"
        }

        Rejection{
            className: "StoredFunctorCall5"
        }

        Rejection{
            className: "StoredFunctorPointerCall0"
        }

        Rejection{
            className: "StoredFunctorPointerCall1"
        }

        Rejection{
            className: "StoredFunctorPointerCall2"
        }

        Rejection{
            className: "StoredFunctorPointerCall3"
        }

        Rejection{
            className: "StoredFunctorPointerCall4"
        }

        Rejection{
            className: "StoredFunctorPointerCall5"
        }

        Rejection{
            className: "StoredMemberFunctionCall0"
        }

        Rejection{
            className: "StoredMemberFunctionCall1"
        }

        Rejection{
            className: "StoredMemberFunctionCall2"
        }

        Rejection{
            className: "StoredMemberFunctionCall3"
        }

        Rejection{
            className: "StoredMemberFunctionCall4"
        }

        Rejection{
            className: "StoredMemberFunctionCall5"
        }

        Rejection{
            className: "StoredMemberFunctionPointerCall0"
        }

        Rejection{
            className: "StoredMemberFunctionPointerCall1"
        }

        Rejection{
            className: "StoredMemberFunctionPointerCall2"
        }

        Rejection{
            className: "StoredMemberFunctionPointerCall3"
        }

        Rejection{
            className: "StoredMemberFunctionPointerCall4"
        }

        Rejection{
            className: "StoredMemberFunctionPointerCall5"
        }

        Rejection{
            className: "ThreadEngine"
        }

        Rejection{
            className: "ThreadEngineSemaphore"
        }

        Rejection{
            className: "ThreadEngineStarterBase"
        }

        Rejection{
            className: "ThreadEngineStarter"
        }

        Rejection{
            className: "UnhandledException"
        }

        Rejection{
            className: "VoidStoredConstMemberFunctionCall0"
        }

        Rejection{
            className: "VoidStoredConstMemberFunctionCall1"
        }

        Rejection{
            className: "VoidStoredConstMemberFunctionCall2"
        }

        Rejection{
            className: "VoidStoredConstMemberFunctionCall3"
        }

        Rejection{
            className: "VoidStoredConstMemberFunctionCall4"
        }

        Rejection{
            className: "VoidStoredConstMemberFunctionCall5"
        }

        Rejection{
            className: "VoidStoredConstMemberFunctionPointerCall0"
        }

        Rejection{
            className: "VoidStoredConstMemberFunctionPointerCall1"
        }

        Rejection{
            className: "VoidStoredConstMemberFunctionPointerCall2"
        }

        Rejection{
            className: "VoidStoredConstMemberFunctionPointerCall3"
        }

        Rejection{
            className: "VoidStoredConstMemberFunctionPointerCall4"
        }

        Rejection{
            className: "VoidStoredConstMemberFunctionPointerCall5"
        }

        Rejection{
            className: "VoidStoredFunctorCall0"
        }

        Rejection{
            className: "VoidStoredFunctorCall1"
        }

        Rejection{
            className: "VoidStoredFunctorCall2"
        }

        Rejection{
            className: "VoidStoredFunctorCall3"
        }

        Rejection{
            className: "VoidStoredFunctorCall4"
        }

        Rejection{
            className: "VoidStoredFunctorCall5"
        }

        Rejection{
            className: "VoidStoredFunctorPointerCall0"
        }

        Rejection{
            className: "VoidStoredFunctorPointerCall1"
        }

        Rejection{
            className: "VoidStoredFunctorPointerCall2"
        }

        Rejection{
            className: "VoidStoredFunctorPointerCall3"
        }

        Rejection{
            className: "VoidStoredFunctorPointerCall4"
        }

        Rejection{
            className: "VoidStoredFunctorPointerCall5"
        }

        Rejection{
            className: "VoidStoredMemberFunctionCall0"
        }

        Rejection{
            className: "VoidStoredMemberFunctionCall1"
        }

        Rejection{
            className: "VoidStoredMemberFunctionCall2"
        }

        Rejection{
            className: "VoidStoredMemberFunctionCall3"
        }

        Rejection{
            className: "VoidStoredMemberFunctionCall4"
        }

        Rejection{
            className: "VoidStoredMemberFunctionCall5"
        }

        Rejection{
            className: "VoidStoredMemberFunctionPointerCall0"
        }

        Rejection{
            className: "VoidStoredMemberFunctionPointerCall1"
        }

        Rejection{
            className: "VoidStoredMemberFunctionPointerCall2"
        }

        Rejection{
            className: "VoidStoredMemberFunctionPointerCall3"
        }

        Rejection{
            className: "VoidStoredMemberFunctionPointerCall4"
        }

        Rejection{
            className: "VoidStoredMemberFunctionPointerCall5"
        }

        Rejection{
            className: "internal"
        }

        Rejection{
            className: "StoredFunctorCall"
        }

        Rejection{
            className: "FunctionResolver"
        }

        Rejection{
            className: "FunctionResolverHelper"
        }

        Rejection{
            className: "InvokeResult"
        }

        Rejection{
            className: "MemberFunctionResolver"
        }

        Rejection{
            className: "NonMemberFunctionResolver"
        }

        Rejection{
            className: "NonPromiseTaskResolver"
        }

        Rejection{
            className: "PromiseTaskResolver"
        }

        Rejection{
            className: "StoredFunctionCall"
        }

        Rejection{
            className: "StoredFunctionCallWithPromise"
        }

        Rejection{
            className: "TaskResolver"
        }

        Rejection{
            className: "TaskResolverHelper"
        }

        Rejection{
            className: "TaskStartParameters"
        }

        Rejection{
            className: "ThreadEngineBarrier"
        }

        Rejection{
            className: "ThreadEngineBase"
        }

        Rejection{
            enumName: "ThreadFunctionResult"
        }

        EnumType{
            name: "ReduceOption"
        }

        EnumType{
            name: "FutureResult"
        }

        ObjectType{
            name: "MedianDouble"
        }
        FunctionalType{
            name: "Runnable"
            using: "std::function<void()>"
            generate: false
        }
        FunctionalType{
            name: "Runnable1"
            using: "std::function<void(const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Runnable2"
            using: "std::function<void(const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Runnable3"
            using: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Runnable4"
            using: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Runnable5"
            using: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Runnable6"
            using: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Runnable7"
            using: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Runnable8"
            using: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Runnable9"
            using: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithVoidPromise"
            using: "std::function<void(QPromise<void>&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithVoidPromise1"
            using: "std::function<void(QPromise<void>&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithVoidPromise2"
            using: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithVoidPromise3"
            using: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithVoidPromise4"
            using: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithVoidPromise5"
            using: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithVoidPromise6"
            using: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithVoidPromise7"
            using: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithVoidPromise8"
            using: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithVoidPromise9"
            using: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Callable"
            using: "std::function<QVariant()>"
            generate: false
        }
        FunctionalType{
            name: "Callable1"
            using: "std::function<QVariant(const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Callable2"
            using: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Callable3"
            using: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Callable4"
            using: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Callable5"
            using: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Callable6"
            using: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Callable7"
            using: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Callable8"
            using: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "Callable9"
            using: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithPromise"
            using: "std::function<void(QPromise<QVariant>&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithPromise1"
            using: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithPromise2"
            using: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithPromise3"
            using: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithPromise4"
            using: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithPromise5"
            using: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithPromise6"
            using: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithPromise7"
            using: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithPromise8"
            using: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "RunnableWithPromise9"
            using: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }

        ModifyFunction{
            signature: "run<Function,Args...>(Function&&,Args&&)"
            Instantiation{
                Argument{
                    type: "std::function<QVariant()>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable<ResultType>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant()>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise<ResultType>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&)>(%env, %in);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable1<ResultType,A>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise1<ResultType,A>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable2<ResultType,A,B>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise2<ResultType,A,B>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable3<ResultType,A,B,C>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise3<ResultType,A,B,C>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable4<ResultType,A,B,C,D>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise4<ResultType,A,B,C,D>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable5<ResultType,A,B,C,D,E>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise5<ResultType,A,B,C,D,E>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable6<ResultType,A,B,C,D,E,F>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise6<ResultType,A,B,C,D,E,F>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable7<ResultType,A,B,C,D,E,F,G>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 8; replaceType: "G"; rename: "g"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise7<ResultType,A,B,C,D,E,F,G>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 8; replaceType: "G"; rename: "g"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable8<ResultType,A,B,C,D,E,F,G,H>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 8; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 9; replaceType: "H"; rename: "h"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise8<ResultType,A,B,C,D,E,F,G,H>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 8; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 9; replaceType: "H"; rename: "h"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable9<ResultType,A,B,C,D,E,F,G,H,I>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 8; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 9; replaceType: "H"; rename: "h"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "I"}
                ModifyArgument{index: 10; replaceType: "I"; rename: "i"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise9<ResultType,A,B,C,D,E,F,G,H,I>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 8; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 9; replaceType: "H"; rename: "h"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "I"}
                ModifyArgument{index: 10; replaceType: "I"; rename: "i"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void()>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void()>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&)>(%env, %in);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable1<A>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise1<A>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable2<A,B>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise2<A,B>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable3<A,B,C>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise3<A,B,C>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable4<A,B,C,D>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise4<A,B,C,D>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable5<A,B,C,D,E>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise5<A,B,C,D,E>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable6<A,B,C,D,E,F>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise6<A,B,C,D,E,F>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable7<A,B,C,D,E,F,G>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 8; replaceType: "G"; rename: "g"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise7<A,B,C,D,E,F,G>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 8; replaceType: "G"; rename: "g"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable8<A,B,C,D,E,F,G,H>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 8; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 9; replaceType: "H"; rename: "h"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise8<A,B,C,D,E,F,G,H>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 8; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 9; replaceType: "H"; rename: "h"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable9<A,B,C,D,E,F,G,H,I>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 8; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 9; replaceType: "H"; rename: "h"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "I"}
                ModifyArgument{index: 10; replaceType: "I"; rename: "i"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise9<A,B,C,D,E,F,G,H,I>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 2; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 3; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 4; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 5; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 6; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 7; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 8; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 9; replaceType: "H"; rename: "h"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "I"}
                ModifyArgument{index: 10; replaceType: "I"; rename: "i"}
            }
        }//run<Function,Args...>(Function&&,Args&&)

        ModifyFunction{
            signature: "run<Function,Args...>(QThreadPool*,Function&&,Args&&)"
            Instantiation{
                Argument{
                    type: "std::function<QVariant()>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Callable<ResultType>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant()>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise<ResultType>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&)>(%env, %in);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Callable1<ResultType,A>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise1<ResultType,A>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Callable2<ResultType,A,B>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise2<ResultType,A,B>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Callable3<ResultType,A,B,C>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise3<ResultType,A,B,C>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Callable4<ResultType,A,B,C,D>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise4<ResultType,A,B,C,D>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Callable5<ResultType,A,B,C,D,E>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise5<ResultType,A,B,C,D,E>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Callable6<ResultType,A,B,C,D,E,F>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise6<ResultType,A,B,C,D,E,F>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Callable7<ResultType,A,B,C,D,E,F,G>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 9; replaceType: "G"; rename: "g"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise7<ResultType,A,B,C,D,E,F,G>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 9; replaceType: "G"; rename: "g"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Callable8<ResultType,A,B,C,D,E,F,G,H>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 9; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 20; replaceType: "H"; rename: "h"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise8<ResultType,A,B,C,D,E,F,G,H>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 9; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 20; replaceType: "H"; rename: "h"}
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Callable9<ResultType,A,B,C,D,E,F,G,H,I>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 9; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 20; replaceType: "H"; rename: "h"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "I"}
                ModifyArgument{index: 20; replaceType: "I"; rename: "i"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "ResultType"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<ResultType>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise9<ResultType,A,B,C,D,E,F,G,H,I>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 9; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 20; replaceType: "H"; rename: "h"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "I"}
                ModifyArgument{index: 20; replaceType: "I"; rename: "i"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void()>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Runnable"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void()>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&)>(%env, %in);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Runnable1<A>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise1<A>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Runnable2<A,B>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise2<A,B>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Runnable3<A,B,C>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise3<A,B,C>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Runnable4<A,B,C,D>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise4<A,B,C,D>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Runnable5<A,B,C,D,E>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise5<A,B,C,D,E>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Runnable6<A,B,C,D,E,F>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise6<A,B,C,D,E,F>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Runnable7<A,B,C,D,E,F,G>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 9; replaceType: "G"; rename: "g"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise7<A,B,C,D,E,F,G>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 9; replaceType: "G"; rename: "g"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Runnable8<A,B,C,D,E,F,G,H>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 9; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 20; replaceType: "H"; rename: "h"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise8<A,B,C,D,E,F,G,H>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 9; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 20; replaceType: "H"; rename: "h"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_INIT(%in)%out = qtjambi_cast<jobject>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$Runnable9<A,B,C,D,E,F,G,H,I>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QFUTURE_POINTER_DECL()auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in QFUTURE_POINTER_ARG);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 9; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 20; replaceType: "H"; rename: "h"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "I"}
                ModifyArgument{index: 21; replaceType: "I"; rename: "i"}
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"}
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise9<A,B,C,D,E,F,G,H,I>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 3; replaceType: "A"; rename: "a"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 4; replaceType: "B"; rename: "b"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 5; replaceType: "C"; rename: "c"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 6; replaceType: "D"; rename: "d"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 7; replaceType: "E"; rename: "e"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 8; replaceType: "F"; rename: "f"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 9; replaceType: "G"; rename: "g"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 20; replaceType: "H"; rename: "h"}
                Argument{type: "JObjectWrapper"; isImplicit: true}
                AddTypeParameter{name: "I"}
                ModifyArgument{index: 21; replaceType: "I"; rename: "i"}
            }
        } // run<Function,Args...>(QThreadPool*,Function&&,Args&&)

        FunctionalType{
            name: "KeepFunctor"
            using: "std::function<bool(const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "KeepFunctorV"
            using: "std::function<bool(const QVariant&)>"
            generate: false
        }
        ModifyFunction{
            signature: "filter<Sequence,KeepFunctor>(Sequence&,KeepFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{
                    index: 1
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            auto sequence = std::make_shared<JavaSequence<JObjectWrapper>>(%env, %in, true);
                            auto& %out = *sequence;`}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in, std::move(sequence));"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "filter<Sequence,KeepFunctor>(QThreadPool*,Sequence&,KeepFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            auto sequence = std::make_shared<JavaSequence<JObjectWrapper>>(%env, %in, true);
                            auto& %out = *sequence;`}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in, std::move(sequence));"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "blockingFilter<Sequence,KeepFunctor>(Sequence&,KeepFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{
                    index: 1
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in, true);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "blockingFilter<Sequence,KeepFunctor>(QThreadPool*,Sequence&,KeepFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in, true);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "filtered<Sequence,KeepFunctor>(Sequence&&,KeepFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<T>"}
                ModifyArgument{
                    index: 1
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`JavaSequence<JObjectWrapper> %out(%env, %in);`}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "filtered<Sequence,KeepFunctor>(QThreadPool*,Sequence&&,KeepFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<T>"}
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`JavaSequence<JObjectWrapper> %out(%env, %in);`}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "filtered<Iterator,KeepFunctor>(Iterator,Iterator,KeepFunctor&&)"
            Instantiation{
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<T>"}
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%1) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "filtered<Iterator,KeepFunctor>(QThreadPool*,Iterator,Iterator,KeepFunctor&&)"
            Instantiation{
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<T>"}
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "blockingFiltered<OutputSequence,Iterator,KeepFunctor>(Iterator,Iterator,KeepFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<QVariant>"
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QList<T>"}
                ModifyArgument{
                    index: 1
                    replaceType: "java.util.Collection<T>"
                    rename: "collection"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            JavaSequence<JObjectWrapper> sequence(%env, %in);
                            auto %out = sequence.begin();
                            `}
                    }
                }
                ModifyArgument{
                    index: 2
                    RemoveArgument{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = sequence.end();"}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "QList<QVariant>"
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QList<T>"}
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%1) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "blockingFiltered<OutputSequence,Iterator,KeepFunctor>(QThreadPool*,Iterator,Iterator,KeepFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<QVariant>"
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QList<T>"}
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    rename: "collection"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            JavaSequence<JObjectWrapper> sequence(%env, %in);
                            auto %out = sequence.begin();
                            `}
                    }
                }
                ModifyArgument{
                    index: 3
                    RemoveArgument{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = sequence.end();"}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "QList<QVariant>"
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QList<T>"}
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        /*ModifyFunction{
            signature: "blockingFiltered<Sequence,KeepFunctor>(QThreadPool*,Sequence&&,KeepFunctor&&)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "blockingFiltered<Sequence,KeepFunctor>(Sequence&&,KeepFunctor&&)"
            remove: RemoveFlag.All
        }*/
        FunctionalType{
            name: "ReduceFunctorV"
            using: "std::function<void(QVariant&,const QVariant&)>"
            generate: false
        }
        FunctionalType{
            name: "ReduceFunctorJ"
            using: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
            generate: false
        }
        ModifyFunction{
            signature: "blockingFilteredReduced<ResultType,Sequence,KeepFunctor,ReduceFunctor,InitialValueType,0>(QThreadPool*,Sequence&&,KeepFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "U"}
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U, T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 5; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "blockingFilteredReduced<ResultType,Sequence,KeepFunctor,ReduceFunctor,InitialValueType,0>(Sequence&&,KeepFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "U"}
                ModifyArgument{
                    index: 1
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U, T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 4; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "blockingFilteredReduced<ResultType,Sequence,KeepFunctor,ReduceFunctor>(Sequence&&,KeepFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "blockingFilteredReduced<ResultType,Sequence,KeepFunctor,ReduceFunctor>(QThreadPool*,Sequence&&,KeepFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "blockingFilteredReduced<ResultType,Iterator,KeepFunctor,ReduceFunctor>(QThreadPool*,Iterator,Iterator,KeepFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "blockingFilteredReduced<ResultType,Iterator,KeepFunctor,ReduceFunctor>(Iterator,Iterator,KeepFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "blockingFilteredReduced<ResultType,Iterator,KeepFunctor,ReduceFunctor,InitialValueType,0>(QThreadPool*,Iterator,Iterator,KeepFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "U"}
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 5
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U, T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 6; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "blockingFilteredReduced<ResultType,Iterator,KeepFunctor,ReduceFunctor,InitialValueType,0>(Iterator,Iterator,KeepFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "U"}
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%1) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U, T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 5; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "filteredReduced<ResultType,Sequence,KeepFunctor,ReduceFunctor,InitialValueType,0>(QThreadPool*,Sequence&&,KeepFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<U>"}
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U, T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 5; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "filteredReduced<ResultType,Sequence,KeepFunctor,ReduceFunctor,InitialValueType,0>(Sequence&&,KeepFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<U>"}
                ModifyArgument{
                    index: 1
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U, T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 4; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "filteredReduced<ResultType,Sequence,KeepFunctor,ReduceFunctor>(Sequence&&,KeepFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "filteredReduced<ResultType,Sequence,KeepFunctor,ReduceFunctor>(QThreadPool*,Sequence&&,KeepFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "filteredReduced<ResultType,Iterator,KeepFunctor,ReduceFunctor>(QThreadPool*,Iterator,Iterator,KeepFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "filteredReduced<ResultType,Iterator,KeepFunctor,ReduceFunctor>(Iterator,Iterator,KeepFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "filteredReduced<ResultType,Iterator,KeepFunctor,ReduceFunctor,InitialValueType,0>(QThreadPool*,Iterator,Iterator,KeepFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<U>"}
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 5
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U, T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 6; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "filteredReduced<ResultType,Iterator,KeepFunctor,ReduceFunctor,InitialValueType,0>(Iterator,Iterator,KeepFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<U>"}
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%1) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U, T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 5; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        FunctionalType{
            name: "MapFunctor"
            using: "std::function<void(const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "MapFunctorV"
            using: "std::function<void(const QVariant&)>"
            generate: false
        }
        ModifyFunction{
            signature: "map<Sequence,MapFunctor>(Sequence&&,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{
                    index: 1
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.concurrent.QtConcurrent$MapFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "map<Sequence,MapFunctor>(QThreadPool*,Sequence&&,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$MapFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "map<Iterator,MapFunctor>(QThreadPool*,Iterator,Iterator,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$MapFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "map<Iterator,MapFunctor>(Iterator,Iterator,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%1) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$MapFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "blockingMap<Sequence,MapFunctor>(Sequence&&,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{
                    index: 1
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.concurrent.QtConcurrent$MapFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "blockingMap<Sequence,MapFunctor>(QThreadPool*,Sequence&&,MapFunctor)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(const JObjectWrapper&)>&&"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$MapFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "blockingMap<Iterator,MapFunctor>(QThreadPool*,Iterator,Iterator,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$MapFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "blockingMap<Iterator,MapFunctor>(Iterator,Iterator,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%1) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$MapFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        FunctionalType{
            name: "MappedFunctor"
            using: "std::function<JObjectWrapper(const JObjectWrapper&)>"
            generate: false
        }
        FunctionalType{
            name: "MappedFunctorV"
            using: "std::function<QVariant(const QVariant&)>"
            generate: false
        }
        FunctionalType{
            name: "MappedFunctorJV"
            using: "std::function<JObjectWrapper(const QVariant&)>"
            generate: false
        }
        ModifyFunction{
            signature: "mapped<Sequence,MapFunctor>(Sequence&&,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<U>"}
                ModifyArgument{
                    index: 1
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<U,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "mapped<Sequence,MapFunctor>(QThreadPool*,Sequence&&,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<U>"}
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<U,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "mapped<Iterator,MapFunctor>(QThreadPool*,Iterator,Iterator,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<U>"}
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<U,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "mapped<Iterator,MapFunctor>(Iterator,Iterator,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<U>"}
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%1) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<U,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "blockingMapped<OutputSequence,InputSequence,MapFunctor>(InputSequence&&,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QList<U>"}
                ModifyArgument{
                    index: 1
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<U,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "blockingMapped<OutputSequence,InputSequence,MapFunctor>(QThreadPool*,InputSequence&&,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QList<U>"}
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<U,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "blockingMapped<Sequence,Iterator,MapFunctor>(QThreadPool*,Iterator,Iterator,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QList<U>"}
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<U,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "blockingMapped<Sequence,Iterator,MapFunctor>(Iterator,Iterator,MapFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QList<U>"}
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%1) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<U,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "mappedReduced<ResultType,Sequence,MapFunctor,ReduceFunctor,InitialValueType,0>(QThreadPool*,Sequence&&,MapFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "V"}
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<U>"}
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<V,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U,V>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 5; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "mappedReduced<ResultType,Sequence,MapFunctor,ReduceFunctor,InitialValueType,0>(Sequence&&,MapFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "V"}
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<U>"}
                ModifyArgument{
                    index: 1
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<V,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U,V>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 4; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "mappedReduced<ResultType,Sequence,MapFunctor,ReduceFunctor>(Sequence&&,MapFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "mappedReduced<ResultType,Sequence,MapFunctor,ReduceFunctor>(QThreadPool*,Sequence&&,MapFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "mappedReduced<ResultType,Iterator,MapFunctor,ReduceFunctor>(QThreadPool*,Iterator,Iterator,MapFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "mappedReduced<ResultType,Iterator,MapFunctor,ReduceFunctor>(Iterator,Iterator,MapFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "mappedReduced<ResultType,Iterator,MapFunctor,ReduceFunctor,InitialValueType,0>(QThreadPool*,Iterator,Iterator,MapFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "V"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<U>"}
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<V,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 5
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U,V>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 6; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "mappedReduced<ResultType,Iterator,MapFunctor,ReduceFunctor,InitialValueType,0>(Iterator,Iterator,MapFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "V"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "io.qt.core.@NonNull QFuture<U>"}
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%1) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<V,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U,V>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 5; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "blockingMappedReduced<ResultType,Sequence,MapFunctor,ReduceFunctor,InitialValueType,0>(QThreadPool*,Sequence&&,MapFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "V"}
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "U"}
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<V,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U,V>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 5; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "blockingMappedReduced<ResultType,Sequence,MapFunctor,ReduceFunctor,InitialValueType,0>(Sequence&&,MapFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "V"}
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "U"}
                ModifyArgument{
                    index: 1
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JavaSequence<JObjectWrapper> %out(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<V,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U,V>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 4; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "blockingMappedReduced<ResultType,Sequence,MapFunctor,ReduceFunctor>(Sequence&&,MapFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "blockingMappedReduced<ResultType,Sequence,MapFunctor,ReduceFunctor>(QThreadPool*,Sequence&&,MapFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "blockingMappedReduced<ResultType,Iterator,MapFunctor,ReduceFunctor>(QThreadPool*,Iterator,Iterator,MapFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "blockingMappedReduced<ResultType,Iterator,MapFunctor,ReduceFunctor>(Iterator,Iterator,MapFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "blockingMappedReduced<ResultType,Iterator,MapFunctor,ReduceFunctor,InitialValueType,0>(QThreadPool*,Iterator,Iterator,MapFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "V"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "U"}
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<V,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 5
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U,V>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 6; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "blockingMappedReduced<ResultType,Iterator,MapFunctor,ReduceFunctor,InitialValueType,0>(Iterator,Iterator,MapFunctor&&,ReduceFunctor&&,InitialValueType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "V"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{index: 0; replaceType: "U"}
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%1) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<V,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U,V>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 5; ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"} }
            }
        }
        ModifyFunction{
            signature: "startMap<Iterator,Functor>(QThreadPool*,Iterator,Iterator,Functor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.concurrent.QtConcurrent$@NonNull ThreadEngineStarter<@QtPrimitiveType Void>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = starter.convert(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    rename: "collection"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            if(!__qt_%1)
                                __qt_%1 = QThreadPool::globalInstance();
                            QtConcurrent::ThreadEngineStarterWrapper starter;
                            auto& sequence = starter.sequence<JObjectWrapper>(%env, %in, true);
                            auto %out = sequence.begin();`}
                    }
                }
                ModifyArgument{
                    index: 3
                    RemoveArgument{}
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`auto %out = sequence.end();`}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$MapFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&)>(%env, %in, starter);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.concurrent.QtConcurrent$@NonNull ThreadEngineStarter<T>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = starter.convert(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QtConcurrent::ThreadEngineStarterWrapper starter;
                            applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$MapFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&)>(%env, %in, starter);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "startMapped<T,Sequence,Functor>(QThreadPool*,Sequence&&,Functor&&)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.concurrent.QtConcurrent$@NonNull ThreadEngineStarter<U>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = starter.convert(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            if(!__qt_%1)
                                __qt_%1 = QThreadPool::globalInstance();
                            QtConcurrent::ThreadEngineStarterWrapper starter;
                            JavaSequence<JObjectWrapper> %out(%env, %in);`}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<U,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in, starter);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "startMapped<T,Iterator,Functor>(QThreadPool*,Iterator,Iterator,Functor&&)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.concurrent.QtConcurrent$@NonNull ThreadEngineStarter<U>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = starter.convert(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QtConcurrent::ThreadEngineStarterWrapper starter;
                            applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<U,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in, starter);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "startMappedReduced<IntermediateType,ResultType,Sequence,MapFunctor,ReduceFunctor>(QThreadPool*,Sequence&&,MapFunctor&&,ReduceFunctor&&,ResultType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                }
                Argument{
                    type: "JObjectWrapper"
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "V"}
                AddTypeParameter{name: "T"}
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.concurrent.QtConcurrent$@NonNull ThreadEngineStarter<U>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = starter.convert(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            if(!__qt_%1)
                                __qt_%1 = QThreadPool::globalInstance();
                            QtConcurrent::ThreadEngineStarterWrapper starter;
                            JavaSequence<JObjectWrapper> %out(%env, %in);`}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<V,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in, starter);"}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U,V>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in, starter);"}
                    }
                }
                ModifyArgument{
                    index: 5
                    ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JObjectWrapper %out = qtjambi_cast<JObjectWrapper>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "startMappedReduced<IntermediateType,ResultType,Iterator,MapFunctor,ReduceFunctor>(QThreadPool*,Iterator,Iterator,MapFunctor&&,ReduceFunctor&&,ResultType&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                }
                Argument{
                    type: "JObjectWrapper"
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<JObjectWrapper(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "V"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.concurrent.QtConcurrent$@NonNull ThreadEngineStarter<U>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = starter.convert(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QtConcurrent::ThreadEngineStarterWrapper starter;
                            applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$MappedFunctor<V,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<JObjectWrapper(const JObjectWrapper&)>(%env, %in, starter);"}
                    }
                }
                ModifyArgument{
                    index: 5
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U,V>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in, starter);"}
                    }
                }
                ModifyArgument{
                    index: 6
                    ReplaceType{modifiedType: "U"; modifiedJavaType: "java.lang.Object"}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "JObjectWrapper %out = qtjambi_cast<JObjectWrapper>(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "startFiltered<Iterator,KeepFunctor>(QThreadPool*,Iterator,Iterator,KeepFunctor&&)"
            Instantiation{
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.concurrent.QtConcurrent$@NonNull ThreadEngineStarter<T>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = starter.convert(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QtConcurrent::ThreadEngineStarterWrapper starter;
                            applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in, starter);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "startFiltered<Sequence,KeepFunctor>(QThreadPool*,Sequence&&,KeepFunctor&&)"
            Instantiation{
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.concurrent.QtConcurrent$@NonNull ThreadEngineStarter<T>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = starter.convert(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            if(!__qt_%1)
                                __qt_%1 = QThreadPool::globalInstance();
                            QtConcurrent::ThreadEngineStarterWrapper starter;
                            JavaSequence<JObjectWrapper> %out(%env, %in);`}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in, starter);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "startFilteredReduced<ResultType,Sequence,MapFunctor,ReduceFunctor>(QThreadPool*,Sequence&&,MapFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QList<JObjectWrapper>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.concurrent.QtConcurrent$@NonNull ThreadEngineStarter<U>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = starter.convert(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    replaceType: "java.util.Collection<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            if(!__qt_%1)
                                __qt_%1 = QThreadPool::globalInstance();
                            QtConcurrent::ThreadEngineStarterWrapper starter;
                            JavaSequence<JObjectWrapper> %out(%env, %in);`}
                    }
                }
                ModifyArgument{
                    index: 3
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in, starter);"}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in, starter);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "startFilteredReduced<ResultType,Iterator,MapFunctor,ReduceFunctor>(QThreadPool*,Iterator,Iterator,MapFunctor&&,ReduceFunctor&&,QtConcurrent::ReduceOptions)"
            Instantiation{
                Argument{
                    type: "JObjectWrapper"
                }
                Argument{
                    type: "JObjectWrapper"
                    isImplicit: true
                }
                Argument{
                    type: "QVariant"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<bool(const JObjectWrapper&)>"
                    isImplicit: true
                }
                Argument{
                    type: "std::function<void(JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "U"}
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "Container"; extending: "QtObjectInterface"}
                AddTypeParameter{name: "Iterator"; extending: "io.qt.core.QSequentialConstIterator<T,Container>"}
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.concurrent.QtConcurrent$@NonNull ThreadEngineStarter<U>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "%out = starter.convert(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QtConcurrent::ThreadEngineStarterWrapper starter;
                            applyOnIterators(%env, %in, [&](auto %out) mutable {`}
                    }
                }
                InjectCode{
                    target: CodeClass.Native
                    position: Position.End
                    Text{content: String.raw`});`}
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "Iterator"
                        modifiedJavaType: "io.qt.core.QSequentialConstIterator"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> %in_info = ContainerAPI::fromJavaOwner(%env, %in);
                            Q_ASSERT(%in_info.second->isSequentialConstIterator());
                            decltype(__qt_%2) %out(%env, %in, %in_info.first, static_cast<AbstractSequentialConstIteratorAccess*>(%in_info.second));
                            `}
                    }
                }
                ModifyArgument{
                    index: 4
                    replaceType: "io.qt.concurrent.QtConcurrent$KeepFunctor<T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<bool(const JObjectWrapper&)>(%env, %in, starter);"}
                    }
                }
                ModifyArgument{
                    index: 5
                    replaceType: "io.qt.concurrent.QtConcurrent$ReduceFunctor<U,T>"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convert<void(JObjectWrapper&,const JObjectWrapper&)>(%env, %in, starter);"}
                    }
                }
            }
        }

        ObjectType{
            name: "QTaskBuilder"
            template: true
            generate: false
            TemplateArguments{
                arguments: ["std::function<void()>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&)>", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant()>"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&)>", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&)>", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&)>", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"]
            }
            TemplateArguments{
                arguments: ["std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper", "JObjectWrapper"]
            }
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void()>>"
            javaName: "QTaskBuilder_V0"
            Import{template: "TaskBuilderTemplatePlainVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IV1"
            Import{template: "TaskBuilderTemplateArguments1PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&)>,JObjectWrapper>"
            javaName: "QTaskBuilder_V1"
            Import{template: "TaskBuilderTemplatePlainVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IV2"
            Import{template: "TaskBuilderTemplateArguments2PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_V2"
            Import{template: "TaskBuilderTemplatePlainVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IV3"
            Import{template: "TaskBuilderTemplateArguments3PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_V3"
            Import{template: "TaskBuilderTemplatePlainVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IV4"
            Import{template: "TaskBuilderTemplateArguments4PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_V4"
            Import{template: "TaskBuilderTemplatePlainVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IV5"
            Import{template: "TaskBuilderTemplateArguments5PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_V5"
            Import{template: "TaskBuilderTemplatePlainVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IV6"
            Import{template: "TaskBuilderTemplateArguments6PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_V6"
            Import{template: "TaskBuilderTemplatePlainVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IV7"
            Import{template: "TaskBuilderTemplateArguments7PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_V7"
            Import{template: "TaskBuilderTemplatePlainVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IV8"
            Import{template: "TaskBuilderTemplateArguments8PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_V8"
            Import{template: "TaskBuilderTemplatePlainVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IV9"
            Import{template: "TaskBuilderTemplateArguments9PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_V9"
            Import{template: "TaskBuilderTemplatePlainVoid"}
        }

        ObjectType{
            name: "QTaskBuilder<std::function<QVariant()>>"
            javaName: "QTaskBuilder_T0"
            Import{template: "TaskBuilderTemplatePlainTypedReturn"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IT1"
            Import{template: "TaskBuilderTemplateArguments1PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&)>,JObjectWrapper>"
            javaName: "QTaskBuilder_T1"
            Import{template: "TaskBuilderTemplatePlainTypedReturn"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IT2"
            Import{template: "TaskBuilderTemplateArguments2PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_T2"
            Import{template: "TaskBuilderTemplatePlainTypedReturn"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IT3"
            Import{template: "TaskBuilderTemplateArguments3PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_T3"
            Import{template: "TaskBuilderTemplatePlainTypedReturn"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IT4"
            Import{template: "TaskBuilderTemplateArguments4PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_T4"
            Import{template: "TaskBuilderTemplatePlainTypedReturn"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IT5"
            Import{template: "TaskBuilderTemplateArguments5PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_T5"
            Import{template: "TaskBuilderTemplatePlainTypedReturn"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IT6"
            Import{template: "TaskBuilderTemplateArguments6PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_T6"
            Import{template: "TaskBuilderTemplatePlainTypedReturn"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IT7"
            Import{template: "TaskBuilderTemplateArguments7PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_T7"
            Import{template: "TaskBuilderTemplatePlainTypedReturn"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IT8"
            Import{template: "TaskBuilderTemplateArguments8PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_T8"
            Import{template: "TaskBuilderTemplatePlainTypedReturn"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IT9"
            Import{template: "TaskBuilderTemplateArguments9PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_T9"
            Import{template: "TaskBuilderTemplatePlainTypedReturn"}
        }

        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&)>>"
            javaName: "QTaskBuilder_PV0"
            Import{template: "TaskBuilderTemplatePlainPromiseVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPV1"
            Import{template: "TaskBuilderTemplateArguments1PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&)>,JObjectWrapper>"
            javaName: "QTaskBuilder_PV1"
            Import{template: "TaskBuilderTemplatePlainPromiseVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPV2"
            Import{template: "TaskBuilderTemplateArguments2PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PV2"
            Import{template: "TaskBuilderTemplatePlainPromiseVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPV3"
            Import{template: "TaskBuilderTemplateArguments3PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PV3"
            Import{template: "TaskBuilderTemplatePlainPromiseVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPV4"
            Import{template: "TaskBuilderTemplateArguments4PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PV4"
            Import{template: "TaskBuilderTemplatePlainPromiseVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPV5"
            Import{template: "TaskBuilderTemplateArguments5PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PV5"
            Import{template: "TaskBuilderTemplatePlainPromiseVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPV6"
            Import{template: "TaskBuilderTemplateArguments6PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PV6"
            Import{template: "TaskBuilderTemplatePlainPromiseVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPV7"
            Import{template: "TaskBuilderTemplateArguments7PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PV7"
            Import{template: "TaskBuilderTemplatePlainPromiseVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPV8"
            Import{template: "TaskBuilderTemplateArguments8PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PV8"
            Import{template: "TaskBuilderTemplatePlainPromiseVoid"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPV9"
            Import{template: "TaskBuilderTemplateArguments9PlainV"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PV9"
            Import{template: "TaskBuilderTemplatePlainPromiseVoid"}
        }

        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&)>>"
            javaName: "QTaskBuilder_PT0"
            Import{template: "TaskBuilderTemplatePlainTypedPromise"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPT1"
            Import{template: "TaskBuilderTemplateArguments1PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&)>,JObjectWrapper>"
            javaName: "QTaskBuilder_PT1"
            Import{template: "TaskBuilderTemplatePlainTypedPromise"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPT2"
            Import{template: "TaskBuilderTemplateArguments2PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PT2"
            Import{template: "TaskBuilderTemplatePlainTypedPromise"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPT3"
            Import{template: "TaskBuilderTemplateArguments3PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PT3"
            Import{template: "TaskBuilderTemplatePlainTypedPromise"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPT4"
            Import{template: "TaskBuilderTemplateArguments4PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PT4"
            Import{template: "TaskBuilderTemplatePlainTypedPromise"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPT5"
            Import{template: "TaskBuilderTemplateArguments5PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PT5"
            Import{template: "TaskBuilderTemplatePlainTypedPromise"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPT6"
            Import{template: "TaskBuilderTemplateArguments6PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PT6"
            Import{template: "TaskBuilderTemplatePlainTypedPromise"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPT7"
            Import{template: "TaskBuilderTemplateArguments7PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PT7"
            Import{template: "TaskBuilderTemplatePlainTypedPromise"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPT8"
            Import{template: "TaskBuilderTemplateArguments8PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PT8"
            Import{template: "TaskBuilderTemplatePlainTypedPromise"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>>"
            javaName: "QTaskBuilder_IPT9"
            Import{template: "TaskBuilderTemplateArguments9PlainT"}
        }
        ObjectType{
            name: "QTaskBuilder<std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper,JObjectWrapper>"
            javaName: "QTaskBuilder_PT9"
            Import{template: "TaskBuilderTemplatePlainTypedPromise"}
        }

        ModifyFunction{
            signature: "task<Task>(Task&&)"
            Instantiation{
                Argument{
                    type: "std::function<void()>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "@NonNull QTaskBuilder<@QtPrimitiveType Void>" }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void()>(%env, %in);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "A"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable1<A>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params1<@QtPrimitiveType Void,A>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable2<A,B>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params2<@QtPrimitiveType Void,A,B>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable3<A,B,C>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params3<@QtPrimitiveType Void,A,B,C>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable4<A,B,C,D>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params4<@QtPrimitiveType Void,A,B,C,D>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable5<A,B,C,D,E>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params5<@QtPrimitiveType Void,A,B,C,D,E>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable6<A,B,C,D,E,F>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params6<@QtPrimitiveType Void,A,B,C,D,E,F>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable7<A,B,C,D,E,F,G>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params7<@QtPrimitiveType Void,A,B,C,D,E,F,G>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable8<A,B,C,D,E,F,G,H>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                AddTypeParameter{name: "G"}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params8<@QtPrimitiveType Void,A,B,C,D,E,F,G,H>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Runnable9<A,B,C,D,E,F,G,H,I>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                AddTypeParameter{name: "G"}
                AddTypeParameter{name: "H"}
                AddTypeParameter{name: "I"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params9<@QtPrimitiveType Void,A,B,C,D,E,F,G,H,I>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant()>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "@NonNull QTaskBuilder<T>" }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable<T>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<QVariant()>(%env, %in);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable1<T,A>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<QVariant(const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params1<T,A>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable2<T,A,B>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<QVariant(const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params2<T,A,B>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable3<T,A,B,C>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params3<T,A,B,C>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable4<T,A,B,C,D>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params4<T,A,B,C,D>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable5<T,A,B,C,D,E>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params5<T,A,B,C,D,E>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable6<T,A,B,C,D,E,F>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params6<T,A,B,C,D,E,F>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable7<T,A,B,C,D,E,F,G>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params7<T,A,B,C,D,E,F,G>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable8<T,A,B,C,D,E,F,G,H>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                AddTypeParameter{name: "G"}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params8<T,A,B,C,D,E,F,G,H>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$Callable9<T,A,B,C,D,E,F,G,H,I>"
                    rename: "callable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                AddTypeParameter{name: "G"}
                AddTypeParameter{name: "H"}
                AddTypeParameter{name: "I"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params9<T,A,B,C,D,E,F,G,H,I>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&)>"
                    isImplicit: true
                }
                ModifyArgument{index: 0; replaceType: "@NonNull QTaskBuilder<@QtPrimitiveType Void>" }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<void>&)>(%env, %in);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params1<@QtPrimitiveType Void,A>" }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise1<A>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<void>&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params2<@QtPrimitiveType Void,A,B>" }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise2<A,B>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise3<A,B,C>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params3<@QtPrimitiveType Void,A,B,C>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise4<A,B,C,D>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params4<@QtPrimitiveType Void,A,B,C,D>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise5<A,B,C,D,E>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params5<@QtPrimitiveType Void,A,B,C,D,E>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise6<A,B,C,D,E,F>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params6<@QtPrimitiveType Void,A,B,C,D,E,F>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise7<A,B,C,D,E,F,G>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params7<@QtPrimitiveType Void,A,B,C,D,E,F,G>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise8<A,B,C,D,E,F,G,H>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                AddTypeParameter{name: "G"}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params8<@QtPrimitiveType Void,A,B,C,D,E,F,G,H>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithVoidPromise9<A,B,C,D,E,F,G,H,I>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<void>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                AddTypeParameter{name: "G"}
                AddTypeParameter{name: "H"}
                AddTypeParameter{name: "I"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params9<@QtPrimitiveType Void,A,B,C,D,E,F,G,H,I>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                ModifyArgument{index: 0; replaceType: "@NonNull QTaskBuilder<T>" }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise<T>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<QVariant>&)>(%env, %in);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params1<T,A>" }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise1<T,A>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<QVariant>&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params2<T,A,B>" }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise2<T,A,B>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise3<T,A,B,C>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params3<T,A,B,C>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise4<T,A,B,C,D>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params4<T,A,B,C,D>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise5<T,A,B,C,D,E>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params5<T,A,B,C,D,E>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise6<T,A,B,C,D,E,F>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params6<T,A,B,C,D,E,F>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise7<T,A,B,C,D,E,F,G>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                AddTypeParameter{name: "G"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params7<T,A,B,C,D,E,F,G>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise8<T,A,B,C,D,E,F,G,H>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                AddTypeParameter{name: "G"}
                AddTypeParameter{name: "H"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params8<T,A,B,C,D,E,F,G,H>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.core.QtFuture$RunnableWithPromise9<T,A,B,C,D,E,F,G,H,I>"
                    rename: "runnable"
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = FutureAPI::convertNullable<void(QPromise<QVariant>&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in);"}
                    }
                }
                AddTypeParameter{name: "T"}
                AddTypeParameter{name: "A"}
                AddTypeParameter{name: "B"}
                AddTypeParameter{name: "C"}
                AddTypeParameter{name: "D"}
                AddTypeParameter{name: "E"}
                AddTypeParameter{name: "F"}
                AddTypeParameter{name: "G"}
                AddTypeParameter{name: "H"}
                AddTypeParameter{name: "I"}
                ModifyArgument{index: 0; replaceType: "QTaskBuilder$@NonNull Params9<T,A,B,C,D,E,F,G,H,I>" }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.End
                    ArgumentMap{index: 1; metaName: "%1"}
                    ArgumentMap{index: 0; metaName: "%0"}
                    Text{content: "%0.setTypes(%1);"}
                }
            }
        }//task<Task>(Task&&)

        InjectCode{
            target: CodeClass.Java
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiConcurrent.java"
                quoteAfterLine: "class QtConcurrent___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::startThreadEngine*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Missing instantiations for template method QtConcurrent::QTaskBuilder<*>::withArguments<ExtraArgs...>(ExtraArgs&&)"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::startMapped*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::startFiltered*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::selectIteration', unmatched parameter type 'T'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::advance', unmatched parameter type 'It&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::filterInternal*', unmatched return type 'ThreadEngineStarter<void>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::createFunctor', unmatched return type 'QtConcurrent::SelectMemberFunctor0lt;T,Class>::type'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::createFunctor', unmatched return type 'SelectFunctor0<T,T>::type'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::createFunctor', unmatched return type 'QtConcurrent::SelectMemberFunctor0<T,Class>::type'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::start', unmatched return type ''"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::*', unmatched return type 'QFuture*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * type 'QtPrivate::ExceptionStore'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * type 'QtConcurrent::ThreadEngineBarrier'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'run()' in 'QtConcurrent$ThreadEngineBase'"}
}
