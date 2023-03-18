/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "functionpointer.h"
#include "qtjambiapi.h"
#include "javaapi.h"
#include <QtCore/QFile>
#include <QtCore/QTemporaryDir>
#include <QtCore/QTemporaryFile>
#include <QtCore/QLibrary>
#include <QtCore/QUuid>
#include <QtCore/QDebug>
#include <memory>

typedef decltype(std::declval<QVector<int>>().size()) size_type;

#if Q_CC_MSVC && !defined(QT_DEBUG)
QFunctionPointer template_keep_dummy(QFunctionPointer ptr, ushort){
    return ptr;
}
#endif

void on_null_ptr(){
    if(JniEnvironment env{100}){
        JavaException::raiseNullPointerException(env, "Function pointer is null." QTJAMBI_STACKTRACEINFO );
    }
}

struct LibraryFunction{
private:
    QFunctionPointer* caller;
    FunctionPointerPrivate::FunctionPointerDisposer disposer;
    void enable(FunctionPointerPrivate::FunctionPointerDisposer _disposer, QFunctionPointer _caller){
        *caller = _caller;
        disposer = _disposer;
    }
    void disable(){
        *caller = nullptr;
        disposer = nullptr;
    }
public:
    QFunctionPointer fn;
    LibraryFunction()
        : caller(nullptr), disposer(nullptr), fn(nullptr) {}
    LibraryFunction(const LibraryFunction& f) : caller(f.caller), disposer(f.disposer), fn(f.fn) {}
    LibraryFunction(LibraryFunction&& f) : caller(std::move(f.caller)), disposer(std::move(f.disposer)), fn(std::move(f.fn)) {}
    ~LibraryFunction()
    {
        if(disposer)
            disposer(fn);
    }
    LibraryFunction& operator=(const LibraryFunction& f) {
        caller = f.caller;
        disposer = f.disposer;
        fn = f.fn;
        return *this;
    }
    LibraryFunction& operator=(LibraryFunction&& f) {
        caller = std::move(f.caller);
        disposer = std::move(f.disposer);
        fn = std::move(f.fn);
        return *this;
    }

    void initialize(QFunctionPointer* _caller, QFunctionPointer _fn){
        caller = _caller;
        fn = _fn;
    }
    friend struct LibraryFile;
};

void register_file_by_function(struct LibraryFile* libFile, QFunctionPointer fn);
void unregister_file_by_function(QFunctionPointer fn);

typedef std::vector<QFunctionPointer>::size_type index_type;

struct LibraryFile : public QSharedData{
    LibraryFile(const QString& _name,
                std::vector<LibraryFunction>&& _libFunctions,
                QSet<index_type>&& _freeIndexes,
                std::unique_ptr<QLibrary>& _library):
        name(_name),
        libFunctions(std::move(_libFunctions)),
        freeIndexes(std::move(_freeIndexes)),
        usedIndexes(),
        library(_library.release()){
    }
    ~LibraryFile(){
        QString tmpFile = library->fileName();
        library->unload();
        delete library;
        QFile::remove(tmpFile);
    }
    QFunctionPointer nextFunction(FunctionPointerPrivate::FunctionPointerDisposer disposer,
                                  QFunctionPointer caller){
        if(freeIndexes.isEmpty())
            return nullptr;
        index_type freeIndex = *freeIndexes.cbegin();
        freeIndexes.erase(freeIndexes.cbegin());
        LibraryFunction& lf = libFunctions[freeIndex];
        lf.enable(disposer, caller);
        usedIndexes[quintptr(lf.fn)] = freeIndex;
        register_file_by_function(this, lf.fn);
        return lf.fn;
    }
    bool disposeFunction(QFunctionPointer fn){
        if(usedIndexes.contains(quintptr(fn))){
            index_type usedIndex = usedIndexes.take(quintptr(fn));
            LibraryFunction& lf = libFunctions[usedIndex];
            lf.disable();
            freeIndexes << usedIndex;
            unregister_file_by_function(fn);
        }
        return usedIndexes.isEmpty();
    }
    const QString name;
private:
    std::vector<LibraryFunction> libFunctions;
    QSet<index_type> freeIndexes;
    QHash<quintptr,size_type> usedIndexes;
    QLibrary* library;
    Q_DISABLE_COPY_MOVE(LibraryFile);
};

struct Libraries{
    Libraries():dir(nullptr){}
    Libraries(const QString& templatePath):dir(new QTemporaryDir(templatePath)){}
    ~Libraries(){
        filesByFunction.clear();
        libraries.clear();
    }
    Libraries& swap(Libraries& other){
        libraries.swap(other.libraries);
        filesByFunction.swap(other.filesByFunction);
        dir.swap(other.dir);
        return *this;
    }
    QHash<QString,QVector<QExplicitlySharedDataPointer<LibraryFile>>> libraries;
    QHash<quintptr,QExplicitlySharedDataPointer<LibraryFile>> filesByFunction;
    std::unique_ptr<QTemporaryDir> dir;
    Q_DISABLE_COPY_MOVE(Libraries);
};

Q_GLOBAL_STATIC_WITH_ARGS(Libraries, gLibraries, (QDir::tempPath()+"/qtjambi.fp.XXXXXX"));
Q_GLOBAL_STATIC(QRecursiveMutex, gMutex)

void register_file_by_function(LibraryFile* libFile, QFunctionPointer fn){
    if(!gLibraries.isDestroyed())
        gLibraries->filesByFunction[quintptr(fn)] = libFile;
}

void unregister_file_by_function(QFunctionPointer fn){
    if(!gLibraries.isDestroyed())
        gLibraries->filesByFunction.remove(quintptr(fn));
}

void clearFunctionPointersAtShutdown(){
    Libraries libraries;
    if(!gLibraries.isDestroyed()){
        QMutexLocker locker(gMutex());
        gLibraries->swap(libraries);
    }
}

namespace FunctionPointerPrivate{

QRecursiveMutex* functionPointerLock(){
    return gMutex();
}

typedef QFunctionPointer* (*Initialize)(QFunctionPointer onNull, std::vector<QFunctionPointer>& functions);

#ifdef Q_OS_ANDROID
#define unique_id(id) qHash(QLatin1String((id).name()))
#else
#define unique_id(id) (id).hash_code()
#endif


QFunctionPointer extractFunction(
        QVector<FunctionParamTypeInfo> functionParamTypeInfos,
        const std::type_info& functionTypeId,
        FunctionPointerDisposer disposer,
        QFunctionPointer caller){
    //void(*disposer)(QFunctionPointer) = nullptr;
    QFunctionPointer result = nullptr;
    Q_ASSERT(functionParamTypeInfos.size()>0);
    QString typeName = functionParamTypeInfos.size()==1 || functionParamTypeInfos.size()>7
            ? QString(QLatin1String("GenericFunction"))
            : QString(QLatin1String("Generic%1Function")).arg(functionParamTypeInfos.size()-1);
    QString funTypeName = QtJambiAPI::typeName(functionTypeId);
    //qDebug() << "trying to find function " << funTypeName;
    if(!QFileInfo::exists(":/io/qt/qtjambi/functionpointers/"+funTypeName)){
        for(const FunctionParamTypeInfo& info : functionParamTypeInfos){
            if(!info.isPointer
                    && !info.isReference
                    && !info.isArithmetic
                    && !info.isEnum
                    && info.sizeOfType>0){
                typeName = funTypeName;
                break;
            }
            /*if(info.isArithmetic
                    && (unique_id(info.plainTypeId)==unique_id(typeid(double))
                        || unique_id(info.plainTypeId)==unique_id(typeid(float)))){

            }*/
        }
    }else{
        typeName = funTypeName;
    }
    if(gLibraries->libraries.contains(typeName)){
        const QVector<QExplicitlySharedDataPointer<LibraryFile>>& availableFiles = gLibraries->libraries[typeName];
        for(QExplicitlySharedDataPointer<LibraryFile> availableFile : availableFiles){
            if(availableFile){
                result = availableFile->nextFunction(disposer, caller);
                if(result)
                    return result;
            }
        }
    }

    //qDebug() << "found: " << typeName;
    QString tmpFile = gLibraries->dir->filePath("~"+QUuid::createUuid().toString(QUuid::Id128)+".tmp");
    if(QFile::copy(":/io/qt/qtjambi/functionpointers/"+typeName, tmpFile)){
        std::unique_ptr<QLibrary> library(new QLibrary(tmpFile));
        if(library->load()){
            if(Initialize initialize = Initialize(library->resolve("initialize"))){
                std::vector<QFunctionPointer> functions;
                QFunctionPointer* callers = initialize(on_null_ptr, functions);
                Q_ASSERT(callers);
                QSet<index_type> freeIndexes;
                std::vector<LibraryFunction> libFunctions;
                libFunctions.resize(functions.size());
                for(index_type i=0; i<functions.size(); ++i){
                    freeIndexes << i;
                    LibraryFunction& lf = libFunctions[i];
                    lf.initialize(callers + i, functions[i]);
                }
                LibraryFile* libFile = new LibraryFile(typeName, std::move(libFunctions), std::move(freeIndexes), library);
                result = libFile->nextFunction(disposer, caller);
                gLibraries->libraries[typeName] << QExplicitlySharedDataPointer<LibraryFile>(libFile);
            }else{
                library->unload();
                qWarning() << "Unable to find initialize function for " << typeName << ".";
            }
        }else{
            qWarning() << "Unable to create function pointer. " << library->errorString();
        }
    }else{
        qWarning() << "Unable to copy library to " << tmpFile;
    }
    return result;
}

bool disposeFunction(QFunctionPointer fn){
    if(!gLibraries.isDestroyed()){
        QExplicitlySharedDataPointer<LibraryFile> libFile = gLibraries->filesByFunction[quintptr(fn)];
        if(libFile){
            bool notInUse = libFile->disposeFunction(fn);
            if(notInUse){
                QVector<QExplicitlySharedDataPointer<LibraryFile>>& files = gLibraries->libraries[libFile->name];
                if(files.size()>1){
                    files.removeAll(libFile);
                }
            }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            libFile = nullptr;
#else
            libFile.reset();
#endif
            return true;
        }
    }
    return false;
}

void noFunctionAvailable(const std::type_info& functionTypeId){
    if(JniEnvironment env{100}){
        Java::Runtime::NullPointerException::throwNew(env, QStringLiteral("Function pointer %1 is null.").arg(QLatin1String(QtJambiAPI::typeName(functionTypeId))) QTJAMBI_STACKTRACEINFO );
    }
}

} // namespace FunctionPointerPrivate
