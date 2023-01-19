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
    packageName: "io.qt.concurrent"
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtConcurrent"
    module: "qtjambi.concurrent"
    description: "Classes for writing multi-threaded programs without using low-level threading primitives."
    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        Text{content: "void initialize_meta_info_util();"}
    }
    
    InjectCode{
        target: CodeClass.MetaInfo
        Text{content: "initialize_meta_info_util();"}
    }
    
    Rejection{
        className: "QtConcurrent"
        enumName: "enum_1"
    }
    
    Rejection{
        className: "QtConcurrent"
        functionName: "operator|"
    }
    
    Rejection{
        className: "QtConcurrent"
        functionName: "run"
    }
    
    Rejection{
        className: "QtConcurrent"
        functionName: "runWithPromise"
    }
    
    Rejection{
        className: "QtConcurrent"
        functionName: "task"
    }
    
    Rejection{
        className: "QtConcurrent"
        functionName: "startFiltered"
    }
    
    Rejection{
        className: "QtConcurrent"
        functionName: "selectIteration"
    }
    
    Rejection{
        className: "QtConcurrent"
        functionName: "blockingFiltered"
    }
    
    Rejection{
        className: "QtConcurrent"
        functionName: "startMap"
    }
    
    Rejection{
        className: "QtConcurrent"
        functionName: "startMappedReduced"
    }
    
    Rejection{
        className: "QtConcurrent"
        functionName: "startMapped"
    }
    
    Rejection{
        className: "QtConcurrent"
        functionName: "startThreadEngine"
    }
    
    Rejection{
        className: "QtConcurrent"
        functionName: "startFilteredReduced"
    }
    
    Rejection{
        className: "QtConcurrent"
        functionName: "filterInternal"
    }
    
    Rejection{
        className: "QtConcurrent::BlockSizeManager"
    }
    
    Rejection{
        className: "QtConcurrent::BlockSizeManagerV2"
    }
    
    Rejection{
        className: "QtConcurrent::BlockSizeManagerV2"
    }
    
    Rejection{
        className: "QtConcurrent::ConstMemberFunctionWrapper"
    }
    
    Rejection{
        className: "QtConcurrent::Exception"
    }
    
    Rejection{
        className: "QtConcurrent::FilterKernel"
    }
    
    Rejection{
        className: "QtConcurrent::FilteredEachKernel"
    }
    
    Rejection{
        className: "QtConcurrent::FilteredReducedKernel"
    }
    
    Rejection{
        className: "QtConcurrent::FunctionWrapper0"
    }
    
    Rejection{
        className: "QtConcurrent::FunctionWrapper1"
    }
    
    Rejection{
        className: "QtConcurrent::FunctionWrapper2"
    }
    
    Rejection{
        className: "QtConcurrent::IntermediateResults"
    }
    
    Rejection{
        className: "QtConcurrent::DefaultValueContainer"
    }
    
    Rejection{
        className: "QtConcurrent::IterateKernel"
    }
    
    Rejection{
        className: "QtConcurrent::MapKernel"
    }
    
    Rejection{
        className: "QtConcurrent::MappedEachKernel"
    }
    
    Rejection{
        className: "QtConcurrent::MappedReducedKernel"
    }
    
    Rejection{
        className: "QtConcurrent::Median"
    }
    
    Rejection{
        className: "QtConcurrent::MemberFunctionWrapper"
    }
    
    Rejection{
        className: "QtConcurrent::MemberFunctionWrapper1"
    }
    
    Rejection{
        className: "QtConcurrent::qValueType"
    }
    
    Rejection{
        className: "QtConcurrent::ReduceKernel"
    }
    
    Rejection{
        className: "QtConcurrent::ResultItem"
    }
    
    Rejection{
        className: "QtConcurrent::ResultIterator"
    }
    
    Rejection{
        className: "QtConcurrent::ResultIteratorBase"
    }
    
    Rejection{
        className: "QtConcurrent::ResultReporter"
    }
    
    Rejection{
        className: "QtConcurrent::ResultStore"
    }
    
    Rejection{
        className: "QtConcurrent::ResultStoreBase"
    }
    
    Rejection{
        className: "QtConcurrent::RunFunctionTask"
    }
    
    Rejection{
        className: "QtConcurrent::RunFunctionTaskBase"
    }
    
    Rejection{
        className: "QtConcurrent::SelectSpecialization"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredConstMemberFunctionCall0"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredConstMemberFunctionCall1"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredConstMemberFunctionCall2"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredConstMemberFunctionCall3"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredConstMemberFunctionCall4"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredConstMemberFunctionCall5"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredConstMemberFunctionPointerCall0"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredConstMemberFunctionPointerCall1"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredConstMemberFunctionPointerCall2"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredConstMemberFunctionPointerCall3"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredConstMemberFunctionPointerCall4"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredConstMemberFunctionPointerCall5"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredFunctorCall0"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredFunctorCall1"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredFunctorCall2"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredFunctorCall3"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredFunctorCall4"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredFunctorCall5"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredFunctorPointerCall0"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredFunctorPointerCall1"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredFunctorPointerCall2"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredFunctorPointerCall3"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredFunctorPointerCall4"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredFunctorPointerCall5"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredMemberFunctionCall0"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredMemberFunctionCall1"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredMemberFunctionCall2"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredMemberFunctionCall3"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredMemberFunctionCall4"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredMemberFunctionCall5"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredMemberFunctionPointerCall0"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredMemberFunctionPointerCall1"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredMemberFunctionPointerCall2"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredMemberFunctionPointerCall3"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredMemberFunctionPointerCall4"
    }
    
    Rejection{
        className: "QtConcurrent::SelectStoredMemberFunctionPointerCall5"
    }
    
    Rejection{
        className: "QtConcurrent::SequenceHolder1"
    }
    
    Rejection{
        className: "QtConcurrent::SequenceHolder2"
    }
    
    Rejection{
        className: "QtConcurrent::StoredConstMemberFunctionCall0"
    }
    
    Rejection{
        className: "QtConcurrent::StoredConstMemberFunctionCall1"
    }
    
    Rejection{
        className: "QtConcurrent::StoredConstMemberFunctionCall2"
    }
    
    Rejection{
        className: "QtConcurrent::StoredConstMemberFunctionCall3"
    }
    
    Rejection{
        className: "QtConcurrent::StoredConstMemberFunctionCall4"
    }
    
    Rejection{
        className: "QtConcurrent::StoredConstMemberFunctionCall5"
    }
    
    Rejection{
        className: "QtConcurrent::StoredConstMemberFunctionPointerCall0"
    }
    
    Rejection{
        className: "QtConcurrent::StoredConstMemberFunctionPointerCall1"
    }
    
    Rejection{
        className: "QtConcurrent::StoredConstMemberFunctionPointerCall2"
    }
    
    Rejection{
        className: "QtConcurrent::StoredConstMemberFunctionPointerCall3"
    }
    
    Rejection{
        className: "QtConcurrent::StoredConstMemberFunctionPointerCall4"
    }
    
    Rejection{
        className: "QtConcurrent::StoredConstMemberFunctionPointerCall5"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctorCall0"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctorCall1"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctorCall2"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctorCall3"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctorCall4"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctorCall5"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctorPointerCall0"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctorPointerCall1"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctorPointerCall2"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctorPointerCall3"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctorPointerCall4"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctorPointerCall5"
    }
    
    Rejection{
        className: "QtConcurrent::StoredMemberFunctionCall0"
    }
    
    Rejection{
        className: "QtConcurrent::StoredMemberFunctionCall1"
    }
    
    Rejection{
        className: "QtConcurrent::StoredMemberFunctionCall2"
    }
    
    Rejection{
        className: "QtConcurrent::StoredMemberFunctionCall3"
    }
    
    Rejection{
        className: "QtConcurrent::StoredMemberFunctionCall4"
    }
    
    Rejection{
        className: "QtConcurrent::StoredMemberFunctionCall5"
    }
    
    Rejection{
        className: "QtConcurrent::StoredMemberFunctionPointerCall0"
    }
    
    Rejection{
        className: "QtConcurrent::StoredMemberFunctionPointerCall1"
    }
    
    Rejection{
        className: "QtConcurrent::StoredMemberFunctionPointerCall2"
    }
    
    Rejection{
        className: "QtConcurrent::StoredMemberFunctionPointerCall3"
    }
    
    Rejection{
        className: "QtConcurrent::StoredMemberFunctionPointerCall4"
    }
    
    Rejection{
        className: "QtConcurrent::StoredMemberFunctionPointerCall5"
    }
    
    Rejection{
        className: "QtConcurrent::ThreadEngine"
    }
    
    Rejection{
        className: "QtConcurrent::ThreadEngineSemaphore"
    }
    
    Rejection{
        className: "QtConcurrent::ThreadEngineStarter"
    }
    
    Rejection{
        className: "QtConcurrent::ThreadEngineStarterBase"
    }
    
    Rejection{
        className: "QtConcurrent::UnhandledException"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredConstMemberFunctionCall0"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredConstMemberFunctionCall1"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredConstMemberFunctionCall2"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredConstMemberFunctionCall3"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredConstMemberFunctionCall4"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredConstMemberFunctionCall5"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredConstMemberFunctionPointerCall0"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredConstMemberFunctionPointerCall1"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredConstMemberFunctionPointerCall2"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredConstMemberFunctionPointerCall3"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredConstMemberFunctionPointerCall4"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredConstMemberFunctionPointerCall5"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredFunctorCall0"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredFunctorCall1"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredFunctorCall2"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredFunctorCall3"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredFunctorCall4"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredFunctorCall5"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredFunctorPointerCall0"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredFunctorPointerCall1"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredFunctorPointerCall2"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredFunctorPointerCall3"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredFunctorPointerCall4"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredFunctorPointerCall5"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredMemberFunctionCall0"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredMemberFunctionCall1"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredMemberFunctionCall2"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredMemberFunctionCall3"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredMemberFunctionCall4"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredMemberFunctionCall5"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredMemberFunctionPointerCall0"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredMemberFunctionPointerCall1"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredMemberFunctionPointerCall2"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredMemberFunctionPointerCall3"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredMemberFunctionPointerCall4"
    }
    
    Rejection{
        className: "QtConcurrent::VoidStoredMemberFunctionPointerCall5"
    }
    
    Rejection{
        className: "QtConcurrent::internal"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctorCall"
    }
    
    Rejection{
        className: "QtConcurrent::FunctionResolver"
    }
    
    Rejection{
        className: "QtConcurrent::FunctionResolverHelper"
    }
    
    Rejection{
        className: "QtConcurrent::InvokeResult"
    }
    
    Rejection{
        className: "QtConcurrent::MemberFunctionResolver"
    }
    
    Rejection{
        className: "QtConcurrent::NonMemberFunctionResolver"
    }
    
    Rejection{
        className: "QtConcurrent::NonPromiseTaskResolver"
    }
    
    Rejection{
        className: "QtConcurrent::PromiseTaskResolver"
    }
    
    Rejection{
        className: "QtConcurrent::QTaskBuilder"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctionCall"
    }
    
    Rejection{
        className: "QtConcurrent::StoredFunctionCallWithPromise"
    }
    
    Rejection{
        className: "QtConcurrent::TaskResolver"
    }
    
    Rejection{
        className: "QtConcurrent::TaskResolverHelper"
    }
    
    Rejection{
        className: "QtConcurrent::TaskStartParameters"
    }
    
    Rejection{
        className: "QtConcurrent::ThreadEngineBarrier"
    }
    
    Rejection{
        className: "QtConcurrent::ThreadEngineBase"
    }
    
    Rejection{
        className: "QtPrivate::ExceptionStore"
    }
    
    NamespaceType{
        name: "QtConcurrent"
        targetType: "class"
        ExtraIncludes{
            Include{
                fileName: "QtConcurrent/QtConcurrent"
                location: Include.Global
            }
        }
        ExtraIncludes{
            Include{
                fileName: "io.qt.core.*"
                location: Include.Java
            }
            Include{
                fileName: "java.util.*"
                location: Include.Java
            }
            Include{
                fileName: "java.util.concurrent.*"
                location: Include.Java
            }
        }
        InjectCode{
            target: CodeClass.Java
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiConcurrent.java"
                quoteAfterLine: "class QtConcurrent___"
                quoteBeforeLine: "}// class"
            }
        }
        InjectCode{
            target: CodeClass.Java
            until: [5, 15]
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiConcurrent.java"
                quoteAfterLine: "class QtConcurrent_5__"
                quoteBeforeLine: "}// class"
            }
        }
        InjectCode{
            target: CodeClass.Java
            since: 6
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiConcurrent.java"
                quoteAfterLine: "class QtConcurrent_6__"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    EnumType{
        name: "QtConcurrent::ReduceOption"
        flags: "QtConcurrent::ReduceOptions"
    }
    
    EnumType{
        name: "QtConcurrent::ThreadFunctionResult"
    }
    
    EnumType{
        name: "QtConcurrent::FutureResult"
    }
    
    ObjectType{
        name: "QtConcurrent::MedianDouble"
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::selectIteration', unmatched parameter type 'T'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::advance', unmatched parameter type 'It&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingMapped', unmatched return type 'Sequence'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingMapped', unmatched return type 'QList<U>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingMapped', unmatched return type 'QList<MapFunctor::result_type>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingMapped', unmatched return type 'OutputSequence'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::filtered', unmatched parameter type 'Iterator'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::filtered', unmatched parameter type 'const Sequence&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::filter', unmatched parameter type 'Sequence&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::startFiltered', unmatched return type 'ThreadEngineStarter<qValueType<Iterator>::value_type>"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::startFiltered', unmatched return type 'ThreadEngineStarter<Sequence::value_type>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::startFiltered', unmatched return type 'ThreadEngineStarter<Iterator::value_type>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingMappedReduced', unmatched return type 'V'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingMappedReduced', unmatched return type 'W'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingMappedReduced', unmatched return type 'ResultType'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingMappedReduced', unmatched return type 'U'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingFiltered', unmatched return type 'OutputSequence'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingFiltered', unmatched return type 'Sequence'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::startMappedReduced*', unmatched return type 'ThreadEngineStarter<ResultType>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingFilteredReduced', unmatched return type 'U'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingFilteredReduced', unmatched return type 'V'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingFilteredReduced', unmatched return type 'ResultType'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::startMap*', unmatched return type 'ThreadEngineStarter<void>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::startThreadEngine*', unmatched return type 'ThreadEngineStarter<ThreadEngine::ResultType>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::mappedReduced', unmatched parameter type 'Iterator'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::mappedReduced', unmatched parameter type 'const Sequence&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::filteredReduced', unmatched parameter type 'Iterator'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::filteredReduced', unmatched parameter type 'const Sequence&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::map', unmatched parameter type 'Iterator'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::map', unmatched parameter type 'Sequence&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::filterInternal*', unmatched return type 'ThreadEngineStarter<void>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::createFunctor', unmatched return type 'QtConcurrent::SelectMemberFunctor0lt;T,Class>::type'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::createFunctor', unmatched return type 'SelectFunctor0<T,T>::type'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::mapped', unmatched parameter type 'Iterator'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::mapped', unmatched parameter type 'const Sequence&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingMap', unmatched parameter type 'Iterator'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingMap', unmatched parameter type 'Sequence&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::startMapped*', unmatched return type 'QtConcurrent::ThreadEngineStarter<T>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::startFilteredReduced*', unmatched return type 'ThreadEngineStarter<ResultType>'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::run', unmatched parameter type 'Class const*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::run', unmatched parameter type 'Class*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::run', unmatched parameter type 'const Class&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::run', unmatched parameter type 'FunctionObject*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::run', unmatched parameter type 'FunctionObject'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::run', unmatched parameter type 'T'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::blockingFilter', unmatched parameter type 'Sequence&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::createFunctor', unmatched return type 'QtConcurrent::SelectMemberFunctor0<T,Class>::type'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::start', unmatched return type ''"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QtConcurrent::*', unmatched return type 'QFuture*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * type 'QtPrivate::ExceptionStore'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * type 'QtConcurrent::ThreadEngineBarrier'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'run()' in 'QtConcurrent$ThreadEngineBase'"}
}
