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
    defaultSuperClass: "QtObject"
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

        Rejection{enumName: "enum_1"}
        Rejection{functionName: "operator|"}
        Rejection{functionName: "run"}
        Rejection{functionName: "runWithPromise"}
        Rejection{functionName: "task"}
        Rejection{functionName: "startFiltered"}
        Rejection{functionName: "selectIteration"}
        Rejection{functionName: "blockingFiltered"}
        Rejection{functionName: "startMap"}
        Rejection{functionName: "startMappedReduced"}
        Rejection{functionName: "startMapped"}
        Rejection{functionName: "startThreadEngine"}
        Rejection{functionName: "startFilteredReduced"}
        Rejection{functionName: "filterInternal"}
        Rejection{functionName: "blockingMappedReduced"}
        Rejection{functionName: "blockingFilter"}
        Rejection{functionName: "mappedReduced"}
        Rejection{functionName: "mapped"}
        Rejection{functionName: "filter"}
        Rejection{functionName: "blockingMapped"}
        Rejection{functionName: "filtered"}
        Rejection{functionName: "filteredReduced"}
        Rejection{functionName: "blockingMap"}
        Rejection{functionName: "map"}
        Rejection{functionName: "blockingFilteredReduced"}
        Rejection{
            className: "BlockSizeManager"
        }

        Rejection{
            className: "BlockSizeManagerV2"
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
            className: "ThreadEngineStarter"
        }

        Rejection{
            className: "ThreadEngineStarterBase"
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
            className: "QTaskBuilder"
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

        EnumType{
            name: "ReduceOption"
        }

        EnumType{
            name: "ThreadFunctionResult"
        }

        EnumType{
            name: "FutureResult"
        }

        ObjectType{
            name: "MedianDouble"
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
