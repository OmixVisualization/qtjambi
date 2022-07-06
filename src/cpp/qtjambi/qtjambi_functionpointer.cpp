#include "qtjambi_functionpointer.h"
#include "qtjambi_core.h"
#include <QtCore/QFile>
#include <QtCore/QTemporaryDir>
#include <QtCore/QTemporaryFile>
#include <QtCore/QLibrary>
#include <QtCore/QUuid>
#include <QtCore/QDebug>
#include <memory>

typedef decltype(std::declval<QVector<int>>().size()) size_type;

void qtjambi_on_null_ptr(){
    if(JNIEnv *env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 100)
        JavaException::raiseNullPointerException(env, "Function pointer is null." QTJAMBI_STACKTRACEINFO );
    }
}

struct LibraryFunction{
private:
    QFunctionPointer* caller;
    QtJambiPrivate::FunctionPointerDisposer disposer;
    void enable(QtJambiPrivate::FunctionPointerDisposer _disposer, QFunctionPointer _caller){
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

struct LibraryFile : public QSharedData{
    LibraryFile(const QString& _name,
                QVector<LibraryFunction>&& _libFunctions,
                QSet<size_type>&& _freeIndexes,
                std::unique_ptr<QLibrary>& _library):
        name(_name),
        libFunctions(_libFunctions),
        freeIndexes(_freeIndexes),
        usedIndexes(),
        library(_library.release()){
    }
    ~LibraryFile(){
        QString tmpFile = library->fileName();
        library->unload();
        delete library;
        QFile::remove(tmpFile);
    }
    QFunctionPointer nextFunction(QtJambiPrivate::FunctionPointerDisposer disposer,
                                  QFunctionPointer caller){
        if(freeIndexes.isEmpty())
            return nullptr;
        size_type freeIndex = *freeIndexes.cbegin();
        freeIndexes.erase(freeIndexes.cbegin());
        LibraryFunction& lf = libFunctions[freeIndex];
        lf.enable(disposer, caller);
        usedIndexes[quintptr(lf.fn)] = freeIndex;
        register_file_by_function(this, lf.fn);
        return lf.fn;
    }
    bool disposeFunction(QFunctionPointer fn){
        if(usedIndexes.contains(quintptr(fn))){
            size_type usedIndex = usedIndexes.take(quintptr(fn));
            LibraryFunction& lf = libFunctions[usedIndex];
            lf.disable();
            freeIndexes << usedIndex;
            unregister_file_by_function(fn);
        }
        return usedIndexes.isEmpty();
    }
    const QString name;
private:
    QVector<LibraryFunction> libFunctions;
    QSet<size_type> freeIndexes;
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

void clear_functionpointers_at_shutdown(){
    Libraries libraries;
    if(!gLibraries.isDestroyed()){
        QMutexLocker locker(gMutex());
        gLibraries->swap(libraries);
    }
}

namespace QtJambiPrivate{

QRecursiveMutex* qtjambi_function_pointer_lock(){
    return gMutex();
}

typedef QFunctionPointer* (*Initialize)(QFunctionPointer onNull, QVector<QFunctionPointer>& functions);

#ifdef Q_OS_ANDROID
#define unique_id(id) qHash(QLatin1String((id).name()))
#else
#define unique_id(id) (id).hash_code()
#endif


QFunctionPointer qtjambi_extract_function(
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
    QString funTypeName = qtjambi_type_name(functionTypeId);
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

    QString tmpFile = gLibraries->dir->filePath("~"+QUuid::createUuid().toString(QUuid::Id128)+".tmp");
    if(QFile::copy(":/io/qt/qtjambi/functionpointers/"+typeName, tmpFile)){
        std::unique_ptr<QLibrary> library(new QLibrary(tmpFile));
        if(library->load()){
            if(Initialize initialize = Initialize(library->resolve("initialize"))){
                QVector<QFunctionPointer> functions;
                QFunctionPointer* callers = initialize(qtjambi_on_null_ptr, functions);
                Q_ASSERT(callers);
                QSet<size_type> freeIndexes;
                QVector<LibraryFunction> libFunctions;
                libFunctions.resize(functions.size());
                for(size_type i=0; i<functions.size(); ++i){
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

bool qtjambi_dispose_function(QFunctionPointer fn){
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

void qtjambi_no_function_available(const std::type_info& functionTypeId){
    if(JNIEnv *env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 100)
        JavaException::raiseNullPointerException(env, qPrintable(QString("Function pointer %1 is null.").arg(QLatin1String(qtjambi_type_name(functionTypeId)))) QTJAMBI_STACKTRACEINFO );
    }
}

} // namespace QtJambiPrivate
