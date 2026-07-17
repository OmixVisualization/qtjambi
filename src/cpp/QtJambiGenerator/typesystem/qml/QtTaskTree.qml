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
    packageName: "io.qt.tasktree"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtTaskTree"
    module: "qtjambi.tasktree"
    description: "Qt TaskTree provides a declarative way to compose and execute asynchronous task workflows with proper error handling and execution policies."
    LoadTypeSystem{name: "QtCore"}

    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        Text{content: "void initialize_meta_info_QtTaskTree_impl();"}
    }

    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.End
        Text{content: "initialize_meta_info_QtTaskTree_impl();"}
    }
    CodeTemplate{
        name: "ifelse.ctr.overloads"
        ImportFile{
            name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
            quoteAfterLine: "class IF_ELSE{"
            quoteBeforeLine: "}// class"
        }
    }
    CodeTemplate{
        name: "group.overloads"
        ImportFile{
            name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
            quoteAfterLine: "class group_overloads{"
            quoteBeforeLine: "}// class"
        }
    }
    CodeTemplate{
        name: "runner.start.overloads"
        ImportFile{
            name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
            quoteAfterLine: "class runner_start_overloads{"
            quoteBeforeLine: "}// class"
        }
    }
    
    NamespaceType{
        name: "QtTaskTree"
        Rejection{className: "ObjectSignal"}
        Rejection{functionName: "onGroupSetup"}
        Rejection{functionName: "onGroupDone"}
        Rejection{functionName: "isInvocable"}
        Rejection{functionName: "wrapTreeSetupHandler"}
        Rejection{functionName: "wrapTreeDoneHandler"}
        EnumType{
            name: "WorkflowPolicy"
        }
        EnumType{
            name: "SetupResult"
        }
        EnumType{
            name: "DoneResult"
        }
        EnumType{
            name: "DoneWith"
        }
        EnumType{
            name: "CallDoneFlag"
        }
        NamespacePrefix{
            namingPolicy: NamespacePrefix.Cut
        }
        ObjectType{
            name: "Iterator"
            FunctionalType{
                name: "Condition"
                generate: false
            }
            FunctionalType{
                name: "ValueGetter"
                generate: false
            }
        }
        ObjectType{
            name: "ForeverIterator"
        }
        ObjectType{
            name: "RepeatIterator"
        }
        ObjectType{
            name: "UntilIterator"
            ExtraIncludes{
                Include{
                    fileName: "QtJambi/JObjectWrapper"
                    location: Include.Global
                }
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
            }
            ModifyFunction{
                signature: "UntilIterator(QtTaskTree::Iterator::Condition)"
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "java.util.function.@NonNull LongPredicate"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            std::function<bool(qsizetype)> %out;
                            if(%in){
                                %out = [wrapper = JObjectWrapper(%env, %in)](qsizetype value) -> bool {
                                        if(JniEnvironment env{200}){
                                            QTJAMBI_TRY{
                                                return Java::Runtime::LongPredicate::test(env, wrapper.object(env), value);
                                            }QTJAMBI_CATCH(const JavaException& exn){
                                                exn.report(env);
                                            }QTJAMBI_TRY_END
                                        }
                                        return false;
                                    };
                            }
                            `}
                    }
                }
            }
        }
        ObjectType{
            name: "ListIterator"
            template: true
            TemplateArguments{
                arguments: ["QVariant"]
            }
        }
        ObjectType{
            name: "ListIterator<QVariant>"
            forceFinal: true
            isGeneric: true
            generate: "no-shell"
            ExtraIncludes{
                Include{
                    fileName: "QtJambi/JObjectWrapper"
                    location: Include.Global
                }
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
            }
            ModifyFunction{
                signature: "ListIterator<RangeGetter,true,RangeType>(RangeGetter&&)"
                remove: RemoveFlag.All
                since: [6,12]
            }
            ModifyFunction{
                signature: "ListIterator(QList<T>)"
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "io.qt.core.QList<T>"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            QPair<void*,AbstractContainerAccess*> pair = ContainerAPI::fromJavaOwner(%env, %in);
                            QPair<const void*,AbstractListAccess*> %out;
                            if(pair.second && pair.second->isList()){
                                %out.first = pair.first;
                                %out.second = static_cast<AbstractListAccess*>(pair.second);
                            }
                            `}
                    }
                }
            }
            ModifyFunction{
                signature: "ListIterator(QtTaskTree::ListIterator<QVariant>)"
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "io.qt.tasktree.ListIterator<T>"
                    }
                    NoNullPointer{}
                }
            }
            ModifyFunction{
                signature: "operator*() const"
                rename: "value"
            }
            ModifyFunction{
                signature: "operator->() const"
                remove: RemoveFlag.All
            }
        }
        ValueType{
            name: "GroupItem"
            generate: "no-shell"
            Rejection{className: "GroupSetupHandler"}
            Rejection{className: "GroupDoneHandler"}
            Rejection{className: "TaskAdapterConstructor"}
            Rejection{className: "TaskAdapterDestructor"}
            Rejection{className: "TaskAdapterStarter"}
            Rejection{className: "TaskAdapterSetupHandler"}
            Rejection{className: "TaskAdapterDoneHandler"}
            Rejection{className: "TaskHandler"}
            Rejection{className: "GroupHandler"}
            Rejection{className: "GroupData"}
            Rejection{enumName: "Type"}
            ExtraIncludes{
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
            }
            CustomConstructor{
                type: CustomConstructor.Default
                Text{content: "createGroupItem(placement);"}
            }
            ModifyFunction{
                signature: "GroupItem<StorageStruct>(QtTaskTree::Storage<StorageStruct>)"
                Instantiation{
                    Argument{
                        type: "QVariant"
                        isImplicit: true
                    }
                }
                Instantiation{
                    Argument{
                        type: "QtTaskTree::QStartedBarrier"
                        isImplicit: true
                    }
                    ModifyArgument{
                        index: 1
                        ReplaceType{
                            modifiedType: "io.qt.tasktree.@StrictNonNull QStoredBarrier"
                        }
                    }
                }
            }
            ModifyFunction{
                signature: "GroupItem(std::initializer_list<QtTaskTree::GroupItem>)"
                ModifyArgument{
                    index: 1
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            std::initializer_list<QtTaskTree::GroupItem> %out = convertGroupItems(%env, %in);
                            `}
                    }
                }
            }
        }
        ObjectType{
            name: "Group"
            ExtraIncludes{
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
            }
            Rejection{functionName: "onGroupSetup"}
            Rejection{functionName: "onGroupDone"}
            ModifyFunction{
                signature: "Group(std::initializer_list<QtTaskTree::GroupItem>)"
                ModifyArgument{
                    index: 1
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            std::initializer_list<QtTaskTree::GroupItem> %out = convertGroupItems(%env, %in);
                            `}
                    }
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: String.raw`
@QtUninvokable
private static native io.qt.tasktree.@NonNull GroupItem onGroupSetupImpl(java.util.function.Supplier<io.qt.tasktree.QtTaskTree.SetupResult> handler);
@QtUninvokable
private static native io.qt.tasktree.@NonNull GroupItem onGroupDoneImpl(java.util.function.Function<io.qt.tasktree.QtTaskTree.DoneWith,io.qt.tasktree.QtTaskTree.DoneResult> handler, io.qt.tasktree.QtTaskTree.CallDone callDone);`}
                InsertTemplate{
                    name: "group.overloads"
                    Replace{
                        from: "ONGROUPDONE%"
                        to: "onGroupDoneImpl"
                    }
                    Replace{
                        from: "ONGROUPSETUP%"
                        to: "onGroupSetupImpl"
                    }
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "struct Group{"
                    quoteBeforeLine: "}// class"
                }
            }
        }
        ValueType{
            name: "ExecutableItem"
            generate: "no-shell"
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
            Rejection{functionName: "withCancel"}
            Rejection{functionName: "withAccept"}
            CustomConstructor{
                type: CustomConstructor.Default
                Text{content: "createExecutableItem(placement);"}
            }
            ModifyFunction{
                signature: "operator!(QtTaskTree::ExecutableItem)"
                rename: "not"
            }
            ModifyFunction{
                signature: "operator&&(QtTaskTree::ExecutableItem,QtTaskTree::ExecutableItem)"
                rename: "and"
            }
            ModifyFunction{
                signature: "operator||(QtTaskTree::ExecutableItem,QtTaskTree::ExecutableItem)"
                rename: "or"
            }
            ModifyFunction{
                signature: "operator&&(QtTaskTree::ExecutableItem,QtTaskTree::DoneResult)"
                rename: "and"
            }
            ModifyFunction{
                signature: "operator||(QtTaskTree::ExecutableItem,QtTaskTree::DoneResult)"
                rename: "or"
            }
            FunctionalType{
                name: "WithTimeout"
                using: "std::function<void()>"
                generate: false
            }
            ModifyFunction{
                signature: "withTimeout(std::chrono::milliseconds,std::function<void()>)const"
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "java.lang.Runnable"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        InsertTemplate{
                            name: "core.runnable.function"
                        }
                    }
                }
            }
            InjectCode{
                target: CodeClass.Java
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "class ExecutableItem{"
                    quoteBeforeLine: "}// class"
                }
            }
            InjectCode{
                target: CodeClass.Native
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "struct ExecutableItem{"
                    quoteBeforeLine: "}// class"
                }
            }
        }
        ValueType{
            name: "ExecutionMode"
            generate: "no-shell"
        }
        ValueType{
            name: "ParallelLimit"
            generate: "no-shell"
        }
        ObjectType{
            name: "For"
            ModifyFunction{
                signature: "For(QtTaskTree::Iterator)"
                ModifyArgument{
                    index: 1
                    NoNullPointer{}
                }
            }
            InjectCode{
                target: CodeClass.Java
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "class ThenFunctions{"
                    quoteBeforeLine: "}// class"
                    Replace{
                        from: "ThenItem"
                        to: "Group"
                    }
                    Replace{
                        from: ".then("
                        to: ".apply("
                    }
                    Replace{
                        from: " then("
                        to: " apply("
                    }
                    Replace{
                        from: "new Then("
                        to: "new Do("
                    }
                    Replace{
                        from: "thenItem"
                        to: "doItem"
                    }
                    Replace{
                        from: " Then "
                        to: " Do "
                    }
                }
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "class For{"
                    quoteBeforeLine: "}// class"
                }
            }
        }
        ObjectType{
            name: "Do"
            ExtraIncludes{
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
            }
            ModifyFunction{
                signature: "Do(std::initializer_list<QtTaskTree::GroupItem>)"
                ModifyArgument{
                    index: 1
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            std::initializer_list<QtTaskTree::GroupItem> %out = convertGroupItems(%env, %in);
                            `}
                    }
                }
            }
        }
        ObjectType{
            name: "Forever"
            ExtraIncludes{
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
            }
            ModifyFunction{
                signature: "Forever(std::initializer_list<QtTaskTree::GroupItem>)"
                ModifyArgument{
                    index: 1
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            std::initializer_list<QtTaskTree::GroupItem> %out = convertGroupItems(%env, %in);
                            `}
                    }
                }
            }
        }
        ObjectType{
            name: "QTaskInterface"
        }
        ObjectType{
            name: "QDefaultTaskAdapter"
            generate: false
        }
        ObjectType{
            name: "QTaskTree"
            Rejection{functionName: "onStorageSetup"}
            Rejection{functionName: "onStorageDone"}
            InjectCode{
                target: CodeClass.Java
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "class QTaskTree{"
                    quoteBeforeLine: "}// class"
                }
            }
        }
        ObjectType{
            name: "QTaskTreeTaskAdapter"
        }
        ObjectType{
            name: "QTimeoutTaskAdapter"
            ExtraIncludes{
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
            }
            ModifyFunction{
                signature: "operator()(std::chrono::milliseconds*,QtTaskTree::QTaskInterface*)"
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "io.qt.tasktree.@Nullable Timeout"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            std::chrono::milliseconds* %out = %in ? reinterpret_cast<std::chrono::milliseconds*>(Java::QtTaskTree::Timeout::__qt_directLink(%env, %in)) : nullptr;
                            `}
                    }
                }
            }
        }
        ObjectType{
            name: "QBarrier"
        }
        ObjectType{
            name: "QStartedBarrier"
        }
        ObjectType{
            name: "When"
            ExtraIncludes{
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
                Include{
                    fileName: "QtJambi/JObjectWrapper"
                    location: Include.Global
                }
                Include{
                    fileName: "QtJambi/Template1Cast"
                    location: Include.Global
                }
                Include{
                    fileName: "QtTaskTree/qprocesstask.h"
                    location: Include.Global
                }
            }
            ModifyFunction{
                signature: "When(QtTaskTree::BarrierKickerGetter,QtTaskTree::WorkflowPolicy)"
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedJavaType: "java.util.function.Function"
                        modifiedType: "java.util.function.@NonNull Function<io.qt.tasktree.@NonNull QStoredBarrier, io.qt.tasktree.@NonNull ExecutableItem>"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            std::function<QtTaskTree::ExecutableItem(const QtTaskTree::Storage<QtTaskTree::QStartedBarrier>&)> %out;
                            if(%in){
                                %out = [wrapper = JObjectWrapper(%env, %in)](const QtTaskTree::Storage<QtTaskTree::QStartedBarrier>& value){
                                        if(JniEnvironment env{200}){
                                            QTJAMBI_TRY{
                                                jobject _value = qtjambi_cast<jobject>(env, value);
                                                jobject result = Java::Runtime::Function::apply(env, wrapper.object(env), _value);
                                                QtTaskTree::ExecutableItem* executableItem = qtjambi_cast<QtTaskTree::ExecutableItem*>(env, result);
                                                if(executableItem)
                                                    return *executableItem;
                                            }QTJAMBI_CATCH(const JavaException& exn){
                                                exn.report(env);
                                            }QTJAMBI_TRY_END
                                        }
                                        return executableItem();
                                    };
                            }
                            `}
                    }
                }
                ModifyArgument{
                    index: 2
                    ReplaceDefaultExpression{
                        expression: "io.qt.tasktree.QtTaskTree.WorkflowPolicy.StopOnError"
                    }
                }
            }
            InjectCode{
                target: CodeClass.Java
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "class ThenFunctions{"
                    quoteBeforeLine: "}// class"
                    Replace{
                        from: "ThenItem"
                        to: "Group"
                    }
                    Replace{
                        from: ".then("
                        to: ".apply("
                    }
                    Replace{
                        from: " then("
                        to: " apply("
                    }
                    Replace{
                        from: "new Then("
                        to: "new Do("
                    }
                    Replace{
                        from: "thenItem"
                        to: "doItem"
                    }
                    Replace{
                        from: " Then "
                        to: " Do "
                    }
                }
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "class When{"
                    quoteBeforeLine: "}// class"
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "struct When{"
                    quoteBeforeLine: "}// class"
                }
            }
        }
        FunctionalType{
            name: "BarrierKickerGetter"
            generate: false
            ExtraIncludes{
                Include{
                    fileName: "QtTaskTree/qbarriertask.h"
                    location: Include.Global
                }
            }
        }
        ObjectType{
            name: "If"
            InjectCode{
                target: CodeClass.Java
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "class If{"
                    quoteBeforeLine: "}// class"
                }
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "class ThenFunctions{"
                    quoteBeforeLine: "}// class"
                }
                InsertTemplate{
                    name: "ifelse.ctr.overloads"
                    Replace{
                        from: "IF_ELSE"
                        to: "If"
                    }
                }
            }
        }
        ObjectType{
            name: "ElseIf"
            InjectCode{
                target: CodeClass.Java
                InsertTemplate{
                    name: "ifelse.ctr.overloads"
                    Replace{
                        from: "IF_ELSE"
                        to: "ElseIf"
                    }
                }
            }
        }
        ObjectType{
            name: "Else"
            ExtraIncludes{
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
            }
            ModifyFunction{
                signature: "Else(std::initializer_list<QtTaskTree::GroupItem>)"
                ModifyArgument{
                    index: 1
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            std::initializer_list<QtTaskTree::GroupItem> %out = convertGroupItems(%env, %in);
                            `}
                    }
                }
            }
        }
        ObjectType{
            name: "Then"
            ExtraIncludes{
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
            }
            ModifyFunction{
                signature: "Then(std::initializer_list<QtTaskTree::GroupItem>)"
                ModifyArgument{
                    index: 1
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            std::initializer_list<QtTaskTree::GroupItem> %out = convertGroupItems(%env, %in);
                            `}
                    }
                }
            }
        }
        ObjectType{
            name: "ThenItem"
            InjectCode{
                target: CodeClass.Java
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "class ThenItem{"
                    quoteBeforeLine: "}// class"
                }
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "class ThenFunctions{"
                    quoteBeforeLine: "}// class"
                    Replace{
                        from: "ThenItem"
                        to: "ElseItem"
                    }
                    Replace{
                        from: ".then("
                        to: ".otherwise("
                    }
                    Replace{
                        from: " then("
                        to: " otherwise("
                    }
                    Replace{
                        from: "new Then("
                        to: "new Else("
                    }
                    Replace{
                        from: "thenItem"
                        to: "elseItem"
                    }
                    Replace{
                        from: " Then "
                        to: " Else "
                    }
                }
            }
            ModifyFunction{
                signature: "operator ExecutableItem()const"
                rename: "endif"
                noImplicitArguments: true
            }
        }
        ObjectType{
            name: "ElseItem"
            ModifyFunction{
                signature: "operator ExecutableItem()const"
                rename: "endif"
                noImplicitArguments: true
            }
        }
        ObjectType{
            name: "ElseIfItem"
            InjectCode{
                target: CodeClass.Java
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "class ThenFunctions{"
                    quoteBeforeLine: "}// class"
                }
            }
        }
        ObjectType{
            name: "QNetworkReplyWrapper"
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
        }
        ObjectType{
            name: "QProcessTaskDeleter"
            generate: false
        }
        ObjectType{
            name: "QProcessTaskAdapter"
        }
        FunctionalType{
            name: "TreeSetupHandler"
            generate: false
        }
        FunctionalType{
            name: "TreeDoneHandler"
            generate: false
        }
        ObjectType{
            name: "QSingleTaskTreeRunner"
            ExtraIncludes{
                Include{
                    fileName: "QtJambi/JavaAPI"
                    location: Include.Global
                }
                Include{
                    fileName: "QtJambi/JObjectWrapper"
                    location: Include.Global
                }
            }
            ModifyFunction{
                signature: "start<SetupHandler,DoneHandler>(QtTaskTree::Group,QtTaskTree::TreeSetupHandler&&,DoneHandler&&,QtTaskTree::CallDone)"
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "java.util.function.@NonNull Consumer<io.qt.tasktree.@NonNull QTaskTree>"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        InsertTemplate{
                            name: "core.consumer.function"
                            Replace{
                                from: "%TYPE"
                                to: "QtTaskTree::QTaskTree &"
                            }
                        }
                    }
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "java.util.function.@NonNull BiConsumer<io.qt.tasktree.@NonNull QTaskTree, io.qt.tasktree.QtTaskTree$@NonNull DoneWith>"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        InsertTemplate{
                            name: "core.bicomsumer.function"
                            Replace{
                                from: "%TYPE1"
                                to: "const QtTaskTree::QTaskTree &"
                            }
                            Replace{
                                from: "%TYPE2"
                                to: "QtTaskTree::DoneWith"
                            }
                        }
                    }
                }
                ModifyArgument{
                    index: 2
                    ReplaceDefaultExpression{
                        expression: "io.qt.tasktree.QtTaskTree.WorkflowPolicy.StopOnError"
                    }
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                InsertTemplate{
                    name: "runner.start.overloads"
                    Replace{
                        from: "FIRSTDECL%"
                        to: ""
                    }
                    Replace{
                        from: "FIRSTLINK%"
                        to: ""
                    }
                    Replace{
                        from: "FIRSTARG%"
                        to: ""
                    }
                }
            }
        }
        ObjectType{
            name: "QSequentialTaskTreeRunner"
            ExtraIncludes{
                Include{
                    fileName: "QtJambi/JavaAPI"
                    location: Include.Global
                }
                Include{
                    fileName: "QtJambi/JObjectWrapper"
                    location: Include.Global
                }
            }
            ModifyFunction{
                signature: "enqueue<SetupHandler,DoneHandler>(QtTaskTree::Group,QtTaskTree::TreeSetupHandler&&,DoneHandler&&,QtTaskTree::CallDone)"
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "java.util.function.@NonNull Consumer<io.qt.tasktree.@NonNull QTaskTree>"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        InsertTemplate{
                            name: "core.consumer.function"
                            Replace{
                                from: "%TYPE"
                                to: "QtTaskTree::QTaskTree &"
                            }
                        }
                    }
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "java.util.function.@NonNull BiConsumer<io.qt.tasktree.@NonNull QTaskTree, io.qt.tasktree.QtTaskTree$@NonNull DoneWith>"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        InsertTemplate{
                            name: "core.bicomsumer.function"
                            Replace{
                                from: "%TYPE1"
                                to: "const QtTaskTree::QTaskTree &"
                            }
                            Replace{
                                from: "%TYPE2"
                                to: "QtTaskTree::DoneWith"
                            }
                        }
                    }
                }
                ModifyArgument{
                    index: 2
                    ReplaceDefaultExpression{
                        expression: "io.qt.tasktree.QtTaskTree.WorkflowPolicy.StopOnError"
                    }
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                InsertTemplate{
                    name: "runner.start.overloads"
                    Replace{
                        from: "start"
                        to: "enqueue"
                    }
                    Replace{
                        from: "FIRSTDECL%"
                        to: ""
                    }
                    Replace{
                        from: "FIRSTLINK%"
                        to: ""
                    }
                    Replace{
                        from: "FIRSTARG%"
                        to: ""
                    }
                }
            }
        }
        ObjectType{
            name: "QParallelTaskTreeRunner"
            ExtraIncludes{
                Include{
                    fileName: "QtJambi/JavaAPI"
                    location: Include.Global
                }
                Include{
                    fileName: "QtJambi/JObjectWrapper"
                    location: Include.Global
                }
            }
            ModifyFunction{
                signature: "start<SetupHandler,DoneHandler>(QtTaskTree::Group,QtTaskTree::TreeSetupHandler&&,DoneHandler&&,QtTaskTree::CallDone)"
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "java.util.function.@NonNull Consumer<io.qt.tasktree.@NonNull QTaskTree>"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        InsertTemplate{
                            name: "core.consumer.function"
                            Replace{
                                from: "%TYPE"
                                to: "QtTaskTree::QTaskTree &"
                            }
                        }
                    }
                }
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "java.util.function.@NonNull BiConsumer<io.qt.tasktree.@NonNull QTaskTree, io.qt.tasktree.QtTaskTree$@NonNull DoneWith>"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        InsertTemplate{
                            name: "core.bicomsumer.function"
                            Replace{
                                from: "%TYPE1"
                                to: "const QtTaskTree::QTaskTree &"
                            }
                            Replace{
                                from: "%TYPE2"
                                to: "QtTaskTree::DoneWith"
                            }
                        }
                    }
                }
                ModifyArgument{
                    index: 2
                    ReplaceDefaultExpression{
                        expression: "io.qt.tasktree.QtTaskTree.WorkflowPolicy.StopOnError"
                    }
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                InsertTemplate{
                    name: "runner.start.overloads"
                    Replace{
                        from: "FIRSTDECL%"
                        to: ""
                    }
                    Replace{
                        from: "FIRSTLINK%"
                        to: ""
                    }
                    Replace{
                        from: "FIRSTARG%"
                        to: ""
                    }
                }
            }
        }
        ObjectType{
            name: "QMappedTaskTreeRunner"
            template: true
            TemplateArguments{
                arguments: ["QVariant"]
            }
        }
        ObjectType{
            name: "QMappedTaskTreeRunner<QVariant>"
            forceFinal: true
            isGeneric: true
            generate: "no-shell"
            ExtraIncludes{
                Include{
                    fileName: "QtJambi/JObjectWrapper"
                    location: Include.Global
                }
                Include{
                    fileName: "QtJambi/JavaAPI"
                    location: Include.Global
                }
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
            }
            ModifyFunction{
                signature: "start<SetupHandler,DoneHandler>(QVariant,QtTaskTree::Group,QtTaskTree::TreeSetupHandler&&,DoneHandler&&,QtTaskTree::CallDone)"
                ModifyArgument{
                    index: 3
                    ReplaceType{
                        modifiedType: "java.util.function.@NonNull Consumer<io.qt.tasktree.@NonNull QTaskTree>"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        InsertTemplate{
                            name: "core.consumer.function"
                            Replace{
                                from: "%TYPE"
                                to: "QtTaskTree::QTaskTree &"
                            }
                        }
                    }
                }
                ModifyArgument{
                    index: 4
                    ReplaceType{
                        modifiedType: "java.util.function.@NonNull BiConsumer<io.qt.tasktree.@NonNull QTaskTree, io.qt.tasktree.QtTaskTree$@NonNull DoneWith>"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        InsertTemplate{
                            name: "core.bicomsumer.function"
                            Replace{
                                from: "%TYPE1"
                                to: "const QtTaskTree::QTaskTree &"
                            }
                            Replace{
                                from: "%TYPE2"
                                to: "QtTaskTree::DoneWith"
                            }
                        }
                    }
                }
                ModifyArgument{
                    index: 2
                    ReplaceDefaultExpression{
                        expression: "io.qt.tasktree.QtTaskTree.WorkflowPolicy.StopOnError"
                    }
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                InsertTemplate{
                    name: "runner.start.overloads"
                    Replace{
                        from: "FIRSTDECL%"
                        to: "@NonNull Key key, "
                    }
                    Replace{
                        from: "FIRSTLINK%"
                        to: "java.lang.Object, "
                    }
                    Replace{
                        from: "FIRSTARG%"
                        to: "key, "
                    }
                }
            }
        }
        ObjectType{
            name: "QTcpSocketWrapper"
        }
        ObjectType{
            name: "QThreadFunctionBase"
            ModifyFunction{
                signature: "setThreadPool(QThreadPool*)"
                ModifyArgument{
                    index: 1
                    ReferenceCount{
                        variableName: "__rcThreadPool"
                        action: ReferenceCount.Set
                    }
                }
            }
        }
        ObjectType{
            name: "StorageBase"
            FunctionalType{
                name: "StorageConstructor"
                generate: false
            }
            FunctionalType{
                name: "StorageDestructor"
                generate: false
            }
            FunctionalType{
                name: "StorageHandler"
                generate: false
            }
        }
        ObjectType{
            name: "Storage"
            template: true
            TemplateArguments{
                arguments: ["QVariant"]
            }
            TemplateArguments{
                arguments: ["QtTaskTree::QStartedBarrier"]
            }
        }
        ObjectType{
            name: "Storage<QVariant>"
            forceFinal: true
            isGeneric: true
            generate: "no-shell"
            ExtraIncludes{
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
            }
            threadAffinity: "this->activeStorage() ? this->activeStorage()->value<QObject*>() : nullptr"
            ModifyFunction{
                signature: "Storage()"
                remove: RemoveFlag.All
            }
            ModifyFunction{
                signature: "Storage<FirstArg,Args...,true>(FirstArg,Args)"
                Instantiation{
                    Argument{
                        type: "QVariant"
                    }
                    Argument{
                        type: "jclass"
                    }
                    Argument{
                        type: "jobject"
                    }
                    ModifyArgument{
                        index: 1
                        rename: "%env"
                        RemoveArgument{}
                    }
                    ModifyArgument{
                        index: 2
                        rename: "structType"
                        ReplaceType{
                            modifiedType: "java.lang.Class<StorageStruct>"
                        }
                        NoNullPointer{}
                    }
                    ModifyArgument{
                        index: 3
                        rename: "supplier"
                        ReplaceType{
                            modifiedType: "java.util.function.Supplier<StorageStruct>"
                        }
                        NoNullPointer{}
                    }
                }
            }
            ModifyFunction{
                signature: "operator*() const"
                remove: RemoveFlag.All
            }
            ModifyFunction{
                signature: "operator->() const"
                remove: RemoveFlag.All
            }
            ModifyFunction{
                signature: "activeStorage() const"
                ModifyArgument{
                    index: 0
                    ReplaceType{
                        modifiedType: "io.qt.tasktree.Storage.@Nullable ActiveStorage<StorageStruct>"
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
                            if(%in){
                                jobject variant = qtjambi_cast<jcoreobject>(%env, %in);
                                QtJambiAPI::registerDependency(%env, variant, __this_nativeId);
                                %out = Java::QtTaskTree::Storage$ActiveStorage::newInstance(%env, __qt_this->structType().typedObject<jclass>(%env), variant);
                            }
                            `}
                    }
                }
            }
            InjectCode{
                target: CodeClass.Java
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "class Storage{"
                    quoteBeforeLine: "}// class"
                }
            }
        }
        ObjectType{
            name: "Storage<QtTaskTree::QStartedBarrier>"
            forceFinal: true
            javaName: "QStoredBarrier"
            generate: "no-shell"
            threadAffinity: "activeStorage()"
            ModifyFunction{
                signature: "Storage<FirstArg,Args...,true>(FirstArg,Args)"
                Instantiation{
                    Argument{
                        type: "qsizetype"
                    }
                    Argument{
                        type: "QObject*"
                    }
                }
                Instantiation{
                    Argument{
                        type: "QObject*"
                    }
                }
            }
            ModifyFunction{
                signature: "operator*() const"
                remove: RemoveFlag.All
            }
            ModifyFunction{
                signature: "operator->() const"
                remove: RemoveFlag.All
            }
        }
        ObjectType{
            name: "QCustomTask"
            template: true
            FunctionalType{
                name: "TaskSetupHandler"
                generate: false
            }
            FunctionalType{
                name: "TaskDoneHandler"
                generate: false
            }
            TemplateArguments{
                arguments: ["QtTaskTree::QTaskTree", "QtTaskTree::QTaskTreeTaskAdapter"]
            }
        }
        ObjectType{
            name: "QCustomTask<QtTaskTree::QTaskTree,QtTaskTree::QTaskTreeTaskAdapter>"
            forceFinal: true
            javaName: "QTaskTreeTask"
            //generate: "no-shell"
            generate: false
            ExtraIncludes{
                Include{
                    fileName: "QtJambi/JObjectWrapper"
                    location: Include.Global
                }
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
            }
            FunctionalType{
                name: "TaskSetupHandler"
                using: "std::function<QtTaskTree::SetupResult(QtTaskTree::QTaskTree &)>"
                generate: false
            }
            FunctionalType{
                name: "TaskDoneHandler"
                using: "std::function<QtTaskTree::DoneResult(const QtTaskTree::QTaskTree &, QtTaskTree::DoneWith)>"
                generate: false
            }
            ModifyFunction{
                signature: "QCustomTask(SetupHandler&&,DoneHandler&&,QtTaskTree::CallDone)"
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "java.util.function.@NonNull Function<io.qt.tasktree.@NonNull QTaskTree, io.qt.tasktree.QtTaskTree$@NonNull SetupResult>"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        InsertTemplate{
                            name: "core.monofunction.function"
                            Replace{
                                from: "%RTYPE"
                                to: "QtTaskTree::SetupResult"
                            }
                            Replace{
                                from: "%ATYPE"
                                to: "QtTaskTree::QTaskTree &"
                            }
                        }
                    }
                }
                ModifyArgument{
                    index: 2
                    ReplaceType{
                        modifiedType: "java.util.function.@NonNull BiFunction<io.qt.tasktree.@NonNull QTaskTree, io.qt.tasktree.QtTaskTree$@NonNull DoneWith, io.qt.tasktree.QtTaskTree$@NonNull DoneResult>"
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        InsertTemplate{
                            name: "core.bifunction.function"
                            Replace{
                                from: "%RTYPE"
                                to: "QtTaskTree::DoneResult"
                            }
                            Replace{
                                from: "%ATYPE1"
                                to: "QtTaskTree::QTaskTree &"
                            }
                            Replace{
                                from: "%ATYPE2"
                                to: "QtTaskTree::DoneWith"
                            }
                        }
                    }
                }
            }
        }
        FunctionalType{
            name: "QSyncRunnable"
            using: "std::function<void()>"
            generate: false
        }
        FunctionalType{
            name: "QSyncBoolSupplier"
            using: "std::function<bool()>"
            generate: false
        }
        FunctionalType{
            name: "QSyncDoneResultSupplier"
            using: "std::function<QtTaskTree::DoneResult()>"
            generate: false
        }
        ObjectType{
            name: "QSyncTask"
            ExtraIncludes{
                Include{
                    fileName: "QtJambi/JObjectWrapper"
                    location: Include.Global
                }
            }
            FunctionalType{
                name: "QSyncRunnable"
                using: "std::function<void()>"
                generate: false
            }
            FunctionalType{
                name: "QSyncBoolSupplier"
                using: "std::function<bool()>"
                generate: false
            }
            FunctionalType{
                name: "QSyncDoneResultSupplier"
                using: "std::function<QtTaskTree::DoneResult()>"
                generate: false
            }
            ModifyFunction{
                signature: "QSyncTask<Handler,true>(Handler&&)"
                Instantiation{
                    Argument{
                        type: "std::function<void()>"
                        isImplicit: true
                    }
                    ModifyArgument{
                        index: 1
                        ReplaceType{
                            modifiedType: "java.lang.Runnable"
                        }
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            InsertTemplate{
                                name: "core.runnable.function"
                            }
                        }
                    }
                }
                Instantiation{
                    Argument{
                        type: "std::function<bool()>"
                        isImplicit: true
                    }
                    ModifyArgument{
                        index: 1
                        ReplaceType{
                            modifiedType: "java.util.function.BooleanSupplier"
                        }
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            InsertTemplate{
                                name: "core.boolsupplier.function"
                            }
                        }
                    }
                }
                Instantiation{
                    Argument{
                        type: "std::function<QtTaskTree::DoneResult()>"
                        isImplicit: true
                    }
                    ModifyArgument{
                        index: 1
                        ReplaceType{
                            modifiedType: "java.util.function.Supplier<io.qt.tasktree.QtTaskTree$@NonNull DoneResult>"
                        }
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            InsertTemplate{
                                name: "core.supplier.function"
                                Replace{
                                    from: "%TYPE"
                                    to: "QtTaskTree::DoneResult"
                                }
                            }
                        }
                    }
                }
            }
        }
        ObjectType{
            name: "QThreadFunctionTaskAdapter"
            template: true
            TemplateArguments{
                arguments: ["QVariant"]
            }
            TemplateArguments{
                arguments: ["void"]
            }
        }
        ObjectType{
            name: "QThreadFunctionTaskAdapter<QVariant>"
            isGeneric: true
            forceFinal: true
            ModifyFunction{
                signature: "operator()(QtTaskTree::QThreadFunction<QVariant>*,QtTaskTree::QTaskInterface*)const"
                ModifyArgument{
                    index: 1
                    replaceType: "io.qt.tasktree.@Nullable QThreadFunction<ResultType>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`QtTaskTree::QThreadFunction<QVariant>* %out = qtjambi_cast<QtTaskTree::QThreadFunction<QVariant>*>(%env, %in);`}
                    }
                }
            }
            InjectCode{
                target: CodeClass.Java
                Text{content: String.raw`
                    @SuppressWarnings("unchecked")
                    static <ResultType> Class<QThreadFunctionTaskAdapter<ResultType>> typedClass(){
                        return (Class<QThreadFunctionTaskAdapter<ResultType>>)(Class<?>)QThreadFunctionTaskAdapter.class;
                    }`}
            }
        }
        ObjectType{
            name: "QThreadFunctionTaskAdapter<void>"
            javaName: "QThreadFunctionVoidTaskAdapter"
            forceFinal: true
        }
        ObjectType{
            name: "QThreadFunction"
            template: true
            TemplateArguments{
                arguments: ["QVariant"]
            }
            TemplateArguments{
                arguments: ["void"]
            }
        }

        ObjectType{
            name: "QThreadFunction<QVariant>"
            ExtraIncludes{
                Include{
                    fileName: "QtJambi/JObjectWrapper"
                    location: Include.Global
                }
                Include{
                    fileName: "QtJambi/FutureAPI"
                    location: Include.Global
                }
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
            }
            isGeneric: true
            threadAffinity: "futureWatcher()"
            ModifyFunction{
                signature: "future()const"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@NonNull QFuture<ResultType>"
                }
            }
            ModifyFunction{
                signature: "futureWatcher()const"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@NonNull QFutureWatcher<ResultType>"
                }
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
                signature: "setThreadFunctionData<Function,Args...>(Function&&,Args&&)"
                Instantiation{
                    Argument{
                        type: "std::function<QVariant()>"
                        isImplicit: true
                    }
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Callable<ResultType>"
                        rename: "callable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<QVariant()>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
                        }
                    }
                }
                Instantiation{
                    Argument{
                        type: "std::function<void(QPromise<QVariant>&)>"
                        isImplicit: true
                    }
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Callable1<ResultType,A>"
                        rename: "callable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Callable2<ResultType,A,B>"
                        rename: "callable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Callable3<ResultType,A,B,C>"
                        rename: "callable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Callable4<ResultType,A,B,C,D>"
                        rename: "callable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Callable5<ResultType,A,B,C,D,E>"
                        rename: "callable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Callable6<ResultType,A,B,C,D,E,F>"
                        rename: "callable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Callable7<ResultType,A,B,C,D,E,F,G>"
                        rename: "callable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Callable8<ResultType,A,B,C,D,E,F,G,H>"
                        rename: "callable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Callable9<ResultType,A,B,C,D,E,F,G,H,I>"
                        rename: "callable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<QVariant(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
            }
            InjectCode{
                target: CodeClass.Java
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "class QThreadFunction{"
                    quoteBeforeLine: "}// class"
                }
            }
            InjectCode{
                target: CodeClass.Java
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "struct QThreadFunction{"
                    quoteBeforeLine: "}// class"
                }
            }
        }
        ObjectType{
            name: "QThreadFunction<void>"
            javaName: "QThreadFunctionVoid"
            ExtraIncludes{
                Include{
                    fileName: "QtJambi/JObjectWrapper"
                    location: Include.Global
                }
                Include{
                    fileName: "QtJambi/FutureAPI"
                    location: Include.Global
                }
                Include{
                    fileName: "utils_p.h"
                    location: Include.Local
                }
            }
            threadAffinity: "futureWatcher()"
            ModifyFunction{
                signature: "future()const"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@NonNull QFuture<@QtPrimitiveType Void>"
                }
            }
            ModifyFunction{
                signature: "futureWatcher()const"
                ModifyArgument{
                    index: 0
                    replaceType: "io.qt.core.@NonNull QFutureWatcher<@QtPrimitiveType Void>"
                }
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
            ModifyFunction{
                signature: "setThreadFunctionData<Function,Args...>(Function&&,Args&&)"

                Instantiation{
                    Argument{
                        type: "std::function<void()>"
                        isImplicit: true
                    }
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Runnable"
                        rename: "runnable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<void()>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
                        }
                    }
                }
                Instantiation{
                    Argument{
                        type: "std::function<void(QPromise<void>&)>"
                        isImplicit: true
                    }
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Runnable1<A>"
                        rename: "runnable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Runnable2<A,B>"
                        rename: "runnable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Runnable3<A,B,C>"
                        rename: "runnable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Runnable4<A,B,C,D>"
                        rename: "runnable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Runnable5<A,B,C,D,E>"
                        rename: "runnable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Runnable6<A,B,C,D,E,F>"
                        rename: "runnable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Runnable7<A,B,C,D,E,F,G>"
                        rename: "runnable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Runnable8<A,B,C,D,E,F,G,H>"
                        rename: "runnable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
                    ModifyArgument{
                        index: 1
                        replaceType: "io.qt.core.QtFuture$Runnable9<A,B,C,D,E,F,G,H,I>"
                        rename: "runnable"
                        NoNullPointer{}
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "auto %out = FutureAPI::convert<void(const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&,const JObjectWrapper&)>(%env, %in EXCEPTION_HANDLER_ARG(__qt_this->futureWatcher()));"}
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
            }
            InjectCode{
                target: CodeClass.Java
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "class QThreadFunction{"
                    quoteBeforeLine: "}// class"
                }
            }
            InjectCode{
                target: CodeClass.Java
                ImportFile{
                    name: ":/io/qtjambi/generator/typesystem/QtJambiTaskTree.java"
                    quoteAfterLine: "struct QThreadFunction{"
                    quoteBeforeLine: "}// class"
                }
            }
        }
        InjectCode{
            target: CodeClass.Java
            position: Position.End
            InsertTemplate{
                name: "group.overloads"
                Replace{
                    from: "ONGROUPDONE%"
                    to: "Group.onGroupDone"
                }
                Replace{
                    from: "ONGROUPSETUP%"
                    to: "Group.onGroupSetup"
                }
            }
        }
        ModifyFunction{
            signature: "operator>>(const QtTaskTree::For &, const QtTaskTree::Do &)"
            access: Modification.Friendly
            rename: "apply"
        }
        ModifyFunction{
            signature: "operator>>(const QtTaskTree::When &, const QtTaskTree::Do &)"
            access: Modification.Friendly
            rename: "apply"
        }
        ModifyFunction{
            signature: "operator>>(const QtTaskTree::If &, const QtTaskTree::Then &)"
            rename: "then"
            access: Modification.Friendly
        }
        ModifyFunction{
            signature: "operator>>(const QtTaskTree::ThenItem &, const QtTaskTree::Else &)"
            rename: "otherwise"
            access: Modification.Friendly
        }
        ModifyFunction{
            signature: "operator>>(const QtTaskTree::ThenItem &, const QtTaskTree::ElseIf &)"
            rename: "elif"
            access: Modification.Friendly
        }
        ModifyFunction{
            signature: "operator>>(const QtTaskTree::ElseIfItem &, const QtTaskTree::Then &)"
            rename: "then"
            access: Modification.Friendly
        }
    }
    GlobalFunction{
        signature: "operator>>(const QtTaskTree::For &, const QtTaskTree::Do &)"
        targetType: "QtTaskTree"
    }
    GlobalFunction{
        signature: "operator>>(const QtTaskTree::When &, const QtTaskTree::Do &)"
        targetType: "QtTaskTree"
    }
    GlobalFunction{
        signature: "operator>>(const QtTaskTree::If &, const QtTaskTree::Then &)"
        targetType: "QtTaskTree"
    }
    GlobalFunction{
        signature: "operator>>(const QtTaskTree::ThenItem &, const QtTaskTree::Else &)"
        targetType: "QtTaskTree"
    }
    GlobalFunction{
        signature: "operator>>(const QtTaskTree::ThenItem &, const QtTaskTree::ElseIf &)"
        targetType: "QtTaskTree"
    }
    GlobalFunction{
        signature: "operator>>(const QtTaskTree::ElseIfItem &, const QtTaskTree::Then &)"
        targetType: "QtTaskTree"
    }
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'ExecutionMode' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Missing instantiations for template method QtTaskTree::When::When<Task,Adapter,Deleter,Signal>(QtTaskTree::QCustomTask<Task,Adapter,Deleter>,Signal,QtTaskTree::WorkflowPolicy)"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Missing instantiations for template method QtTaskTree::If::If<Handler,true>(Handler&&)"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Missing instantiations for template method QtTaskTree::ElseIf::ElseIf<Handler,true>(Handler&&)"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: Cloneable class QtTaskTree::ListIterator<QVariant> is missing an explicit copy constructor"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: Cloneable class QtTaskTree::Storage<QVariant> is missing an explicit copy constructor"}
}
