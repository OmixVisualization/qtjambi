/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QUrl>
#include <QtCore/QResource>
#include <QtCore/QScopeGuard>
#include <QtCore/private/qabstractfileengine_p.h>
#include <QtCore/private/qfsfileengine_p.h>
#include "qtjambiapi.h"
#include "jobjectwrapper.h"
#include "java_p.h"
#ifdef Q_OS_ANDROID
#include "androidapi.h"
#endif
#include "utils_p.h"

#ifdef Q_OS_ANDROID
#include <android/asset_manager_jni.h>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

#include "qtjambi_cast.h"

typedef QHash<QString, QSet<QString>> PathsToJarFilesHash;
typedef QHash<QString, QSet<QString>> PathsToDirectoryHash;
typedef QSet<QString> ClassPathDirURLSet;
void truncateBuffer(JNIEnv *env, jobject buffer);

class QClassPathFileEngineHandler: public QAbstractFileEngineHandler
{
public:
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    typedef std::unique_ptr<QAbstractFileEngine> QAbstractFileEnginePointer;
#else
    typedef QAbstractFileEngine* QAbstractFileEnginePointer;
#endif
    QAbstractFileEnginePointer create(const QString &fileName) const override;
    QClassPathFileEngineHandler();
    ~QClassPathFileEngineHandler() override;
    static void insertJarFileResources(JNIEnv *env, jobject directoryPaths, jstring jarFileName);
    static void insertJarFileResource(const QString& directoryPath, const QString& jarFileName);
    static void removeResource(const QString& jarFileName);
    static void addClassPath(const QString& path, bool isDirectory);
    static QSet<QString> jarFilesByDirectory(const QString& directoryPath, QString* directPath = nullptr);
    static QSet<QString> classPathsByDirectory(const QString& topLevelDir);
    static QSet<QString> classPathDirURLs();
private:
    mutable QReadWriteLock m_fileEngineLock;
    PathsToJarFilesHash m_jarFilesByContentDirectories;
    PathsToDirectoryHash m_filePathsByContentDirectories;
    ClassPathDirURLSet m_classPathDirURLSet;
    QString m_qtJambiConfFile;
    friend void ensureHandler(JNIEnv* env, jstring strg);
};

Q_GLOBAL_STATIC(QScopedPointer<QClassPathFileEngineHandler>, gClassPathFileEngineHandler)

QClassPathFileEngineHandler::QClassPathFileEngineHandler()
    : QAbstractFileEngineHandler(), m_qtJambiConfFile()
{
}

QClassPathFileEngineHandler::~QClassPathFileEngineHandler(){
    QWriteLocker locker(&m_fileEngineLock);
    m_jarFilesByContentDirectories.clear();
    m_filePathsByContentDirectories.clear();
    m_classPathDirURLSet.clear();
}

void initializeFileEngineResources(){
    if(!gClassPathFileEngineHandler->get()){
        gClassPathFileEngineHandler->reset(new QClassPathFileEngineHandler());
    }
}

void ensureHandler(JNIEnv* env, jstring strg){
    initializeFileEngineResources();
    if(Q_UNLIKELY(!gClassPathFileEngineHandler.isDestroyed())){
        QClassPathFileEngineHandler* handler = gClassPathFileEngineHandler->get();
        if(Q_LIKELY(handler)){
            QString qtJambiConfFile;
            int length = strg ? env->GetStringLength(strg) : 0;
            qtJambiConfFile.resize(length);
            if(length>0)
                env->GetStringRegion(strg, 0, length, reinterpret_cast<ushort*>(qtJambiConfFile.data()));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            QWriteLocker locker(&handler->m_fileEngineLock);
            handler->m_qtJambiConfFile = qtJambiConfFile;
        }
    }
}

void clearFileEngineResourcesAtShutdown(JNIEnv* env){
    gClassPathFileEngineHandler->reset();
    if(env){
        Java::QtJambi::ResourceUtility::cleanupOnShutdown(env);
    }
}

void QClassPathFileEngineHandler::insertJarFileResources(JNIEnv *env, jobject directoryPaths, jstring _jarFileName){
    if(Q_UNLIKELY(!gClassPathFileEngineHandler.isDestroyed())){
        QClassPathFileEngineHandler* handler = gClassPathFileEngineHandler->get();
        if(Q_LIKELY(handler)){
            const QString& jarFileName = qtjambi_cast<QString>(env, _jarFileName);
            QWriteLocker locker(&handler->m_fileEngineLock);
            jobject iter = Java::Runtime::Collection::iterator(env, directoryPaths);
            while(Java::Runtime::Iterator::hasNext(env, iter)){
                jstring path = jstring(Java::Runtime::Iterator::next(env, iter));
                handler->m_jarFilesByContentDirectories[qtjambi_cast<QString>(env, path)].insert(jarFileName);
            }
        }
    }
}

void QClassPathFileEngineHandler::insertJarFileResource(const QString& directoryPath, const QString& jarFileName){
    if(Q_UNLIKELY(!gClassPathFileEngineHandler.isDestroyed())){
        QClassPathFileEngineHandler* handler = gClassPathFileEngineHandler->get();
        if(Q_LIKELY(handler)){
            QWriteLocker locker(&handler->m_fileEngineLock);
            handler->m_jarFilesByContentDirectories[directoryPath].insert(jarFileName);
        }
    }
}

void QClassPathFileEngineHandler::removeResource(const QString& path){
    if(Q_UNLIKELY(!gClassPathFileEngineHandler.isDestroyed())){
        QClassPathFileEngineHandler* handler = gClassPathFileEngineHandler->get();
        if(Q_LIKELY(handler)){
            QWriteLocker locker(&handler->m_fileEngineLock);
            {
                const QStringList keys = handler->m_jarFilesByContentDirectories.keys();
                for(const QString& key : keys){
                    handler->m_jarFilesByContentDirectories[key].remove(path);
                }
            }
            {
                const QStringList keys = handler->m_filePathsByContentDirectories.keys();
                for(const QString& key : keys){
                    handler->m_filePathsByContentDirectories[key].remove(path);
                }
            }
            handler->m_classPathDirURLSet.remove(path);
        }
    }
}

QSet<QString> QClassPathFileEngineHandler::jarFilesByDirectory(const QString& directoryPath, QString* directPath){
    if(Q_UNLIKELY(!gClassPathFileEngineHandler.isDestroyed())){
        const QClassPathFileEngineHandler* handler = gClassPathFileEngineHandler->get();
        if(Q_LIKELY(handler)){
            if(directPath && !handler->m_qtJambiConfFile.isEmpty() && QStringLiteral(u"etc/qt/qt.conf")==directoryPath){
                *directPath = handler->m_qtJambiConfFile;
                return QSet<QString>{};
            }
            QReadLocker locker(&handler->m_fileEngineLock);
            return handler->m_jarFilesByContentDirectories[directoryPath];
        }
    }
    return QSet<QString>{};
}

QSet<QString> QClassPathFileEngineHandler::classPathsByDirectory(const QString& topLevelDir){
    if(Q_UNLIKELY(!gClassPathFileEngineHandler.isDestroyed())){
        const QClassPathFileEngineHandler* handler = gClassPathFileEngineHandler->get();
        if(Q_LIKELY(handler)){
            QReadLocker locker(&handler->m_fileEngineLock);
            return handler->m_filePathsByContentDirectories[topLevelDir];
        }
    }
    return QSet<QString>{};
}

QSet<QString> QClassPathFileEngineHandler::classPathDirURLs(){
    if(Q_UNLIKELY(!gClassPathFileEngineHandler.isDestroyed())){
        const QClassPathFileEngineHandler* handler = gClassPathFileEngineHandler->get();
        if(Q_LIKELY(handler)){
            QReadLocker locker(&handler->m_fileEngineLock);
            return handler->m_classPathDirURLSet;
        }
    }
    return QSet<QString>{};
}

void QClassPathFileEngineHandler::addClassPath(const QString& path, bool isDirectory){
    if(Q_UNLIKELY(!gClassPathFileEngineHandler.isDestroyed())){
        QClassPathFileEngineHandler* handler = gClassPathFileEngineHandler->get();
        if(Q_LIKELY(handler)){
            PathsToDirectoryHash classPathDirSet;
            if(isDirectory){
                QDir directory(path);
                QFileInfoList fil = directory.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
                for(const QFileInfo& file : fil){
                    if(file.isDir()){
                        classPathDirSet[file.fileName()].insert(path);
                    }else{
                        classPathDirSet[QString()].insert(path);
                    }
                }
            }
            QWriteLocker locker(&handler->m_fileEngineLock);
            for(auto iter = classPathDirSet.keyValueBegin(); iter!=classPathDirSet.keyValueEnd(); ++iter){
                handler->m_filePathsByContentDirectories[iter->first].unite(iter->second);
            }
            if(!isDirectory)
                handler->m_classPathDirURLSet.insert(path);
        }
    }
}

void insertJarFileResources(JNIEnv *env, jobject directoryPaths, jstring jarFileName){
    QClassPathFileEngineHandler::insertJarFileResources(env, directoryPaths, jarFileName);
}

void removeResource(const QString& jarFileName){
    QClassPathFileEngineHandler::removeResource(jarFileName);
}

void addClassPath(const QString& path, bool isDirectory){
    QClassPathFileEngineHandler::addClassPath(path, isDirectory);
}

class QClassPathEntry : public QSharedData{
public:
    virtual ~QClassPathEntry();
};

class QFSEntryEngine final : public QFSFileEngine, public QClassPathEntry {
public:
    QFSEntryEngine(const QString& file)
                        : QFSFileEngine(file),
                          QClassPathEntry()
    {}
private:
};

class QJarEntryEngine final : public QAbstractFileEngine, public QClassPathEntry {
public:
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 1)
    typedef QFileDevice::FileTime FileTime;
    static constexpr FileTime BirthTime = FileTime::FileBirthTime;
    static constexpr FileTime AccessTime = FileTime::FileAccessTime;
    static constexpr FileTime MetadataChangeTime = FileTime::FileMetadataChangeTime;
    static constexpr FileTime ModificationTime = FileTime::FileModificationTime;
#endif
    QJarEntryEngine(JNIEnv* env, jobject myJarFile, const QString& jarFileName, const QString& fileName, const QString& prefix);
    ~QJarEntryEngine() override;

    void setFileName(const QString &file) override;

    bool copy(const QString &newName) override;

    bool caseSensitive() const override {
        return true;
    }

    bool close() override;

    qint64 size() const override;

    bool seek(qint64 offset) override;

    QStringList entryList(QDir::Filters filters, const QStringList &filterNames) const override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    QStringList entryList(QDirListing::IteratorFlags filters, const QStringList &filterNames) const override;
#endif

    FileFlags fileFlags(FileFlags type=FileInfoAll) const override;

    QString fileName(FileName file=DefaultName) const override;

    QDateTime fileTime(FileTime time) const override;

    bool open(QIODevice::OpenMode openMode
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
             ,std::optional<QFile::Permissions> permissions = std::nullopt
#endif
              ) override;

    qint64 pos() const override;

    qint64 read(char *data, qint64 maxlen) override;
    bool isValid() const { return m_valid; }
private:
    bool reset();
    bool reopen();
    bool close(JNIEnv* env);
    bool closeStream();
    bool closeStream(JNIEnv* env);
    QString m_jarFileName;
    QString m_entryFileName;
    JObjectWrapper m_entry;
    JObjectWrapper m_myJarFile;
    JObjectWrapper m_readChannel;
    qint64 m_pos;
    qint64 m_size;
    QIODevice::OpenMode m_openMode;
    bool m_valid;
    bool m_directory;
    QString m_name;
    bool m_closed;
    QString m_prefix;
};

QJarEntryEngine::QJarEntryEngine(JNIEnv* env, jobject myJarFile, const QString& jarFileName, const QString& fileName, const QString& prefix)
    : QAbstractFileEngine(),
      QClassPathEntry(),
      m_jarFileName(jarFileName),
      m_entryFileName(),
      m_entry(),
      m_myJarFile(env, myJarFile),
      m_readChannel(),
      m_pos(-1),
      m_size(0),
      m_openMode(QIODevice::NotOpen),
      m_valid(false),
      m_directory(false),
      m_name(),
      m_closed(false),
      m_prefix(prefix)
{
    QJarEntryEngine::setFileName(fileName);
}

QJarEntryEngine::~QJarEntryEngine(){
    if(JniEnvironment env{400}){
        try {
            QJarEntryEngine::close(env);
        } catch(const JavaException& exn) {
            exn.report(env);
        } catch(...){}
        m_entry.assign(env, nullptr);
        m_readChannel.assign(env, nullptr);
        m_myJarFile.assign(env, nullptr);
    }
}

void QJarEntryEngine::setFileName(const QString &fileName){
    if (m_closed)
         return;

    m_entryFileName = fileName;
    if (m_entryFileName.isEmpty()) {
        m_name = "";
        m_valid = true;
        m_directory = true;
        m_entry = nullptr;
        return;
    }

    if(JniEnvironmentExceptionHandler env{400}){
        try{
            jobject myJarFile = m_myJarFile.object(env);
            jstring entryFileName = qtjambi_cast<jstring>(env, m_entryFileName);
            jobject entry = Java::QtJambi::ResourceUtility$JarResource::getJarEntry(env, myJarFile, entryFileName);
            m_entry.assign(env, entry);

            if (!entry) {
                m_directory = Java::QtJambi::ResourceUtility$JarResource::isDirectory(env, myJarFile, entryFileName);
                m_size = 0;
                if((m_valid = m_directory))
                    m_name = fileName;
            } else {
                m_name = qtjambi_cast<QString>(env, Java::Runtime::ZipEntry::getName(env, entry));
                m_directory = Java::QtJambi::ResourceUtility$JarResource::checkIsDirectory(env, myJarFile, entry);
                m_size = m_directory ? 0 : Java::Runtime::ZipEntry::getSize(env, entry);
                m_valid = true;
            }
        }catch(const JavaException& exn){
            env.handleException(exn, this, "QJarEntryEngine::setFileName(QString)");
        }
    }
}

bool QJarEntryEngine::copy(const QString &newName){

    QFile newFile(newName);
    if (newFile.exists())
        return false;

    if (!open(QFile::ReadOnly))
        return false;

    if (!newFile.open(QFile::WriteOnly)) {
        closeStream();
        return false;
    }

    auto sz = size();
    decltype(sz) i = 0;

    if (sz > 0) {
        const decltype(sz) BUFFER_SIZE = 64*1024;	// 64Kb will do us
        char* buffer = new char[BUFFER_SIZE];
        while (true){
            auto r = read(buffer, BUFFER_SIZE);
            if(r>0){
                i += r;
                newFile.write(buffer, r);
            }else{
                break;
            }
        }
    }

    newFile.close();
    if (!closeStream())
        return false;

    return (i == sz);
}

bool QJarEntryEngine::close(){
    bool result = false;
    if(m_readChannel || m_closed) {
        if(JniEnvironmentExceptionHandler env{600}){
            try{
                result = close(env);
            }catch(const JavaException& exn){
                env.handleException(exn, this, "QJarEntryEngine::close()");
            }
        }
    }
    return result;
}

bool QJarEntryEngine::close(JNIEnv* env){
    bool bf = closeStream(env);

    if(!m_closed) {
        // We really want to do this some how and not leave it to disposed()
        Java::QtJambi::ResourceUtility$JarResource::_deref(env, m_myJarFile.object(env));
        // the reference this instance already had on construction
        // We do not null the reference here, since we could need to reopen() on handle
        // so we record the state separately with boolean m_closed flag.
        m_closed = true;
    }

    return bf;
}

bool QJarEntryEngine::closeStream() {
    if(m_readChannel) {
        if(JniEnvironmentExceptionHandler env{200}){
            try {
                return closeStream(env);
            } catch(const JavaException& exn) {
                env.handleException(exn, this, "QJarEntryEngine::closeStream()");
            }
        }
    }
    return false;
}

bool QJarEntryEngine::closeStream(JNIEnv* env) {
    if(m_readChannel) {
        try {
            Java::Runtime::AutoCloseable::close(env, m_readChannel.object(env));
        } catch(const JavaException&) {
        }
        m_readChannel.assign(env, nullptr);
        try{
            Java::QtJambi::ResourceUtility$JarResource::_deref(env, m_myJarFile.object(env));
        }catch(const JavaException&){
        }
        return true;
    }

    return false;
}

QStringList QJarEntryEngine::entryList(QDir::Filters filters, const QStringList &filterNames) const{
    QStringList result;
    if (m_directory){
        if (!(filters & QDir::NoDotAndDotDot) && (filters & QDir::Dirs)) {
            result << ".";
            if (m_entryFileName.length() > 0)
                result << "..";
        }
        if (!(filters & (QDir::Readable | QDir::Writable | QDir::Executable)))
            filters.setFlag(QDir::Readable);
        if(JniEnvironmentExceptionHandler env{600}){
            try{
                Java::QtJambi::ResourceUtility$JarResource::entryList(env, m_myJarFile.object(env),
                                               qtjambi_cast<jobject>(env, &result),
                                               jint(int(filters)),
                                               qtjambi_cast<jobject>(env, filterNames),
                                               qtjambi_cast<jstring>(env, m_name), false);
            }catch(const JavaException& exn){
                env.handleException(exn, this, "QJarEntryEngine::entryList(QDir::Filters, QStringList)const");
            }
        }
    }
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
QStringList QJarEntryEngine::entryList(QDirListing::IteratorFlags filters, const QStringList &filterNames) const{
    QStringList result;
    if (m_directory){
        if (!(filters & QDirListing::IteratorFlag::IncludeDotAndDotDot) && !(filters & QDirListing::IteratorFlag::FilesOnly)) {
            result << ".";
            if (m_entryFileName.length() > 0)
                result << "..";
        }
        if(JniEnvironmentExceptionHandler env{600}){
            try{
                Java::QtJambi::ResourceUtility$JarResource::entryList(env, m_myJarFile.object(env),
                                                                  qtjambi_cast<jobject>(env, &result),
                                                                  jint(int(filters)),
                                                                  qtjambi_cast<jobject>(env, filterNames),
                                                                  qtjambi_cast<jstring>(env, m_name), true);
            }catch(const JavaException& exn){
                env.handleException(exn, this, "QJarEntryEngine::entryList(QDirListing::IteratorFlags, QStringList)const");
            }
        }
    }
    return result;
}
#endif


QAbstractFileEngine::FileFlags QJarEntryEngine::fileFlags(FileFlags type) const{
    QAbstractFileEngine::FileFlags flags;

    QFileInfo info(m_jarFileName);
     if (info.exists()) {
         flags |= ExistsFlag;
         if(info.isReadable()){
             flags |= ReadUserPerm;
         }
         if(info.isWritable()){
             flags |= WriteUserPerm;
         }
         if(info.isExecutable()){
             flags |= ExeUserPerm;
         }
     }

     if (m_directory)
         flags |= DirectoryType;
     else
         flags |= FileType;


     return FileFlags(flags & type);
}

QString QJarEntryEngine::fileName(FileName file) const{
    QString entryFileName = m_entryFileName;

    QString s;
#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
    if (file == LinkName)
        s = "";
    else
#endif
        if (file == DefaultName
            || file == AbsoluteName
            || file == CanonicalName) {
        s = m_prefix + m_jarFileName + "#" + entryFileName;
    } else if (file == BaseName) {
        auto pos = m_entryFileName.lastIndexOf("/");
        s = pos >= 0 ? m_entryFileName.mid(pos + 1) : entryFileName;
    } else if (file == PathName) {
        auto pos = m_entryFileName.lastIndexOf("/");
        s = pos > 0 ? m_entryFileName.mid(0, pos) : "";
    } else if (file == CanonicalPathName || file == AbsolutePathName) {
        s = m_prefix + m_jarFileName + "#" + fileName(PathName);
    }
    return s;
}

QDateTime QJarEntryEngine::fileTime(FileTime t) const{
    if(JniEnvironmentExceptionHandler env{600}){
        try{
            jlong time = Java::QtJambi::ResourceUtility$JarResource::fileTime(env, m_myJarFile.object(env), m_entry.object(env), t==BirthTime, t==AccessTime, t==MetadataChangeTime || t==ModificationTime);
            if(time>=0)
                return QDateTime::fromMSecsSinceEpoch(time);
        }catch(const JavaException& exn){
            env.handleException(exn, this, "QJarEntryEngine::fileTime(FileTime)const");
        }
    }
    return QDateTime();
}

bool QJarEntryEngine::open(QIODevice::OpenMode openMode
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
         ,std::optional<QFile::Permissions>
#endif
                           ) {
    bool bf = false;
    if (m_entry) {
        if (!openMode.testFlag(QIODevice::WriteOnly) && !openMode.testFlag(QIODevice::Append)) {
            if(JniEnvironmentExceptionHandler env{600}){
                try{
                    closeStream(env);
                    // There is a usage case where the application may have called close() on the handle and is now calling open() again.
                    // This can also happen implicitly if we need to rewind() on the Jar stream
                    int oldRefCount = Java::QtJambi::ResourceUtility$JarResource::ensureRef(env, m_myJarFile.object(env));  // increment reference while we have stream open

                    m_readChannel = Java::QtJambi::ResourceUtility$JarResource::getChannel(env, m_myJarFile.object(env), m_entry.object(env));
                    if (m_readChannel) {
                        m_pos = 0;
                        m_openMode = openMode;
                        m_closed = false;  // this maybe due to reopen() or first open()
                        bf = true;
                    }else{
                        if(oldRefCount >= 0) {
                            if(oldRefCount == 0)
                                Java::QtJambi::ResourceUtility$JarResource::_deref(env, m_myJarFile.object(env)); // rollback extra reference
                            Java::QtJambi::ResourceUtility$JarResource::_deref(env, m_myJarFile.object(env)); // rollback reference
                        }
                    }
                }catch(const JavaException& exn){
                    env.handleException(exn, this, "QJarEntryEngine::open(QIODevice::OpenMode)");
                }
            }
        }else{
            closeStream();
        }
    }
    return bf;
}

qint64 QJarEntryEngine::pos() const {
    return m_pos;
}

qint64 QJarEntryEngine::size() const{
    return m_size;
}

qint64 QJarEntryEngine::read(char *data, qint64 maxlen) {
    qint64 readBytes = 0;
    if(m_pos<m_size && m_readChannel){
        if(JniEnvironmentExceptionHandler env{600}){
            jobject buffer = env->NewDirectByteBuffer(data, maxlen);
            Java::Runtime::ReadableByteChannel::read(env, m_readChannel.object(env), buffer);
            readBytes = Java::Runtime::Internal::Buffer::position(env, buffer);
            truncateBuffer(env, buffer);
        }
    }
    if(readBytes==0)
        return -1;
    m_pos += readBytes;
    return readBytes;
}

bool QJarEntryEngine::reset() {
    if (!m_readChannel)
        return false;  // not open
    if (m_pos == 0)
        return true;  // already open and at start
    return reopen();
}

bool QJarEntryEngine::reopen() {
    if (!m_readChannel)
        return false;  // not open
    QIODevice::OpenMode om = m_openMode;  // saved OpenMode
    if(closeStream())
        return open(om);
    return false;
}

bool QJarEntryEngine::seek(qint64 offset) {
    if(offset < 0)
        return false;
    if(offset==0){
        return reset();
    }
    if(m_pos < offset) {
        QByteArray data;
        data.fill('\0', offset - m_pos);
        read(data.data(), data.size());
        return m_pos == offset;
    }else if(offset < m_size){
        if(reset()){
            QByteArray data;
            data.fill('\0', offset);
            read(data.data(), data.size());
            return m_pos == offset;
        }
    }
    return false;
}

#ifdef Q_OS_ANDROID
class QAssetEntryEngine final : public QAbstractFileEngine, public QClassPathEntry {
public:
    QAssetEntryEngine(AAssetManager *assetManager, const QString& fileName);
    ~QAssetEntryEngine() override;

    void setFileName(const QString &file) override;

    bool copy(const QString &newName) override;

    bool caseSensitive() const override {
        return true;
    }

    bool isRelativePath() const override {
        return false;
    }

    bool close() override;

    qint64 size() const override;

    bool seek(qint64 offset) override;

    QStringList entryList(QDir::Filters filters, const QStringList &filterNames) const override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    QStringList entryList(QDirListing::IteratorFlags filters, const QStringList &filterNames) const override;
#endif

    FileFlags fileFlags(FileFlags type=FileInfoAll) const override;

    QString fileName(FileName file=DefaultName) const override;

    bool open(QIODevice::OpenMode openMode
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
             ,std::optional<QFile::Permissions> permissions = std::nullopt
#endif
              ) override;

    qint64 pos() const override;

    qint64 read(char *data, qint64 maxlen) override;

    bool isValid() const { return m_assetFile; }
private:
    QString m_fileName;
    AAsset *m_assetFile = nullptr;
    AAssetManager *m_assetManager = nullptr;
};

QAssetEntryEngine::QAssetEntryEngine(AAssetManager *assetManager, const QString& fileName)
    : QAbstractFileEngine(),
      QClassPathEntry(),
      m_fileName(),
      m_assetManager(assetManager)
{
    QAssetEntryEngine::setFileName(fileName);
}

QAssetEntryEngine::~QAssetEntryEngine(){
    QAssetEntryEngine::close();
}

void QAssetEntryEngine::setFileName(const QString &_fileName){
    QString fileName = _fileName;
    fileName.replace(QLatin1String("//"), QLatin1String("/"));
    if (fileName.startsWith(QLatin1Char('/')))
        fileName.remove(0, 1);
    if (fileName.endsWith(QLatin1Char('/')))
        fileName.chop(1);
    if(m_fileName==fileName)
        return;
    close();
    m_fileName = fileName;
    open(QIODevice::ReadOnly);
}

bool QAssetEntryEngine::copy(const QString &newName){

    QFile newFile(newName);
    if (newFile.exists())
        return false;

    if (!open(QFile::ReadOnly))
        return false;

    if (!newFile.open(QFile::WriteOnly)) {
        return false;
    }

    auto sz = size();
    decltype(sz) i = 0;

    if (sz > 0) {
        const decltype(sz) BUFFER_SIZE = 64*1024;	// 64Kb will do us
        char* buffer = new char[BUFFER_SIZE];
        while (true){
            auto r = read(buffer, BUFFER_SIZE);
            if(r>0){
                i += r;
                newFile.write(buffer, r);
            }else{
                break;
            }
        }
    }

    newFile.close();
    return (i == sz);
}

bool QAssetEntryEngine::close(){
    if (m_assetFile) {
        AAsset_close(m_assetFile);
        m_assetFile = nullptr;
        return true;
    }
    return false;
}

QStringList QAssetEntryEngine::entryList(QDir::Filters, const QStringList &) const{
    return {};
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
QStringList QAssetEntryEngine::entryList(QDirListing::IteratorFlags, const QStringList &) const{
    return {};
}
#endif

QAbstractFileEngine::FileFlags QAssetEntryEngine::fileFlags(FileFlags type) const{
    FileFlags commonFlags(ReadOwnerPerm|ReadUserPerm|ReadGroupPerm|ReadOtherPerm|ExistsFlag);
    FileFlags flags;
    if (m_assetFile)
        flags = FileType | commonFlags;
    return type & flags;
}

QString QAssetEntryEngine::fileName(FileName file) const{
    int pos;
    switch (file) {
    case DefaultName:
    case AbsoluteName:
    case CanonicalName:
            return QLatin1String(":/") + m_fileName;
    case BaseName:
        if ((pos = m_fileName.lastIndexOf(QChar(QLatin1Char('/')))) != -1)
            return QLatin1String(":/") + m_fileName.mid(pos);
        else
            return QLatin1String(":/") + m_fileName;
    case PathName:
    case AbsolutePathName:
    case CanonicalPathName:
        if ((pos = m_fileName.lastIndexOf(QChar(QLatin1Char('/')))) != -1)
            return QLatin1String(":/") + m_fileName.left(pos);
        else
            return QLatin1String(":/") + m_fileName;
    default:
        return QString();
    }
}

bool QAssetEntryEngine::open(QIODevice::OpenMode openMode
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
         ,std::optional<QFile::Permissions>
#endif
                           ) {
    if (openMode & QIODevice::WriteOnly)
        return false;
    close();
    QByteArray qrcFileName("qrc");
    if(!m_fileName.endsWith("/"))
        qrcFileName += "/";
    qrcFileName += m_fileName.toUtf8();
    m_assetFile = AAssetManager_open(m_assetManager, qrcFileName, AASSET_MODE_BUFFER);
    return m_assetFile;
}

qint64 QAssetEntryEngine::pos() const {
    if (m_assetFile)
        return AAsset_seek(m_assetFile, 0, SEEK_CUR);
    return -1;
}

qint64 QAssetEntryEngine::size() const{
    if (m_assetFile)
        return AAsset_getLength(m_assetFile);
    return -1;
}

qint64 QAssetEntryEngine::read(char *data, qint64 maxlen) {
    if (m_assetFile)
        return AAsset_read(m_assetFile, data, maxlen);
    return -1;
}

bool QAssetEntryEngine::seek(qint64 offset) {
    if (m_assetFile)
        return offset == AAsset_seek(m_assetFile, offset, SEEK_SET);
    return false;
}


class QAssetDirectoryEngine final : public QAbstractFileEngine, public QClassPathEntry {
public:
    QAssetDirectoryEngine(QStringList&& dirEntries, const QString& fileName);
    ~QAssetDirectoryEngine() override;

    bool caseSensitive() const override {
        return true;
    }

    bool isRelativePath() const override {
        return false;
    }

    QStringList entryList(QDir::Filters filters, const QStringList &filterNames) const override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    QStringList entryList(QDirListing::IteratorFlags, const QStringList &) const override;
#endif

    FileFlags fileFlags(FileFlags type=FileInfoAll) const override;

    QString fileName(FileName file=DefaultName) const override;

    bool isValid() const { return true; }
private:
    QString m_fileName;
    QStringList m_dirEntries;
};

QAssetDirectoryEngine::QAssetDirectoryEngine(QStringList&& dirEntries, const QString& fileName)
    : QAbstractFileEngine(),
      QClassPathEntry(),
      m_fileName(fileName),
      m_dirEntries(std::move(dirEntries))
{
}

QAssetDirectoryEngine::~QAssetDirectoryEngine(){
}

QStringList QAssetDirectoryEngine::entryList(QDir::Filters, const QStringList &) const{
    return m_dirEntries;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
QStringList QAssetDirectoryEngine::entryList(QDirListing::IteratorFlags, const QStringList &) const{
    return m_dirEntries;
}
#endif

QAbstractFileEngine::FileFlags QAssetDirectoryEngine::fileFlags(FileFlags type) const{
    FileFlags commonFlags(ReadOwnerPerm|ReadUserPerm|ReadGroupPerm|ReadOtherPerm|ExistsFlag);
    FileFlags flags;
    flags = DirectoryType | commonFlags;
    return type & flags;
}

QString QAssetDirectoryEngine::fileName(FileName file) const{
    int pos;
    switch (file) {
    case DefaultName:
    case AbsoluteName:
    case CanonicalName:
            return QLatin1String(":/") + m_fileName;
    case BaseName:
        if ((pos = m_fileName.lastIndexOf(QChar(QLatin1Char('/')))) != -1)
            return QLatin1String(":/") + m_fileName.mid(pos);
        else
            return QLatin1String(":/") + m_fileName;
    case PathName:
    case AbsolutePathName:
    case CanonicalPathName:
        if ((pos = m_fileName.lastIndexOf(QChar(QLatin1Char('/')))) != -1)
            return QLatin1String(":/") + m_fileName.left(pos);
        else
            return QLatin1String(":/") + m_fileName;
    default:
        return QString();
    }
}
#endif //def Q_OS_ANDROID

class QUrlEntryEngine final : public QAbstractFileEngine, public QClassPathEntry {
public:
    QUrlEntryEngine(JNIEnv* env, jobject url, const QString& fileName);

    ~QUrlEntryEngine() override;

    qint64 read(char *data, qint64 maxlen) override;

    bool open(QIODevice::OpenMode openMode
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
             ,std::optional<QFile::Permissions> permissions = std::nullopt
#endif
              ) override;

    bool close() override;

    qint64 size() const override{
        return m_size;
    }

    QString fileName(FileName=DefaultName) const override{
        return m_fileName;
    }

    bool caseSensitive() const override {
        return true;
    }

    FileFlags fileFlags(FileFlags) const override;
private:
    QString m_fileName;
    JObjectWrapper m_connection;
    JObjectWrapper m_readChannel;
    qint64 m_size;
    FileFlags m_flags;
};

class QClassPathEngine final : public QAbstractFileEngine {

public:
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    typedef QFile::FileTime FileTime;
    typedef IteratorUniquePtr IteratorPtr;
#else
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 1)
    typedef QFileDevice::FileTime FileTime;
#endif
    typedef Iterator* IteratorPtr;
#endif
    QClassPathEngine(QStringList&& resourceEntries);

    ~QClassPathEngine() override;

    void setFileName(const QString &fileName) override;

    bool copy(const QString& newName) override;

    bool setPermissions(uint perms) override;

    bool caseSensitive() const override{
        return true;
    }

    bool close() override;

    FileFlags fileFlags(FileFlags type=FileInfoAll) const override;

    QString fileName(FileName file=DefaultName) const override;

    QDateTime fileTime(FileTime time) const override;

    bool link(const QString& newName) override;

    bool mkdir(const QString& dirName, bool createParentDirectories
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
               ,std::optional<QFile::Permissions> permissions = std::nullopt
#endif
               ) const override;

    bool open(QIODevice::OpenMode openMode
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
             ,std::optional<QFile::Permissions> permissions = std::nullopt
#endif
              ) override;

    qint64 pos() const override;

    qint64 read(char *data, qint64 maxlen) override;

    qint64 readLine(char *data, qint64 maxlen) override;

    bool remove() override;

    bool rename(const QString& newName) override;

    bool rmdir(const QString& dirName, bool recursive) const override;

    bool seek(qint64 offset) override;

    QString owner(FileOwner owner) const override;

    uint ownerId(FileOwner owner) const override;

    bool isRelativePath() const override {
        return false;
    }

    bool isSequential() const override;

    bool setSize(qint64 sz) override;

    qint64 size() const override;

    qint64 write(const char *data, qint64 len) override;

    QStringList entryList(QDir::Filters filters, const QStringList& filterNames) const override;

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    QStringList entryList(QDirListing::IteratorFlags filters, const QStringList& filterNames) const override;
    IteratorPtr beginEntryList(const QString &path, QDirListing::IteratorFlags filters, const QStringList& nameFilters) override;
#else
    IteratorPtr beginEntryList(QDir::Filters filters, const QStringList& nameFilters) override;
#endif

    bool isValid(){return !m_engines.isEmpty();}
private:
    bool addFromPath(JNIEnv* env, jobject url, const QString& fileName, const QString& prefix, QList<QAbstractFileEngine*>& engines);
    QAbstractFileEngine* getFirstEngine() const;

    QString m_prefix;
    QString m_fileName;
    QString m_baseName;
    QString m_selectedSource = "*";
    QList<QAbstractFileEngine*> m_engines;
    const QStringList m_resourceEntries;
    mutable QMutex m_mutex;
};

class QClassPathEngineIterator final : public QAbstractFileEngineIterator{
public:
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    QClassPathEngineIterator(const QString &path, const QStringList& entries, QDirListing::IteratorFlags filters, const QStringList &nameFilters);
    bool advance() override;
#else
    QClassPathEngineIterator(const QStringList& entries, QDir::Filters filters, const QStringList &nameFilters);
    QString next() override;
    bool hasNext() const override;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QFileInfo currentFileInfo() const override;
#endif
#endif
    ~QClassPathEngineIterator() override = default;
    QString currentFileName() const override;
    QListIterator<QString> m_iterator;
    QString m_current;
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
QClassPathEngineIterator::QClassPathEngineIterator(const QString &path, const QStringList& entries, QDirListing::IteratorFlags filters, const QStringList &nameFilters)
    : QAbstractFileEngineIterator(
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
        path,
#endif
        filters, nameFilters),
    m_iterator(entries),
    m_current(m_iterator.hasNext() ? m_iterator.peekNext() : QString())
{
}

bool QClassPathEngineIterator::advance(){
    bool hasNext = m_iterator.hasNext();
    if(hasNext){
        m_current = m_iterator.next();
    }
    return hasNext;
}
#else
QClassPathEngineIterator::QClassPathEngineIterator(const QStringList& entries, QDir::Filters filters, const QStringList &nameFilters)
    : QAbstractFileEngineIterator(
        filters, nameFilters),
    m_iterator(entries),
    m_current(m_iterator.hasNext() ? m_iterator.peekNext() : QString())
{
}

QString QClassPathEngineIterator::next() {
    m_current = m_iterator.next();
    return m_current;
}

bool QClassPathEngineIterator::hasNext() const{
    return m_iterator.hasNext();
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QFileInfo QClassPathEngineIterator::currentFileInfo() const{
    QString name = currentFileName();
    if (!name.isNull()) {
        QString tmp = path();
        if (!tmp.isEmpty()) {
            if (!tmp.endsWith(QLatin1Char('/')))
                tmp.append(QLatin1Char('/'));
            name.prepend(tmp);
        }
    }
    return QFileInfo(name);
}
#endif
#endif

QString QClassPathEngineIterator::currentFileName() const{
    return m_current;
}

QClassPathEntry::~QClassPathEntry(){}

QClassPathEngine::QClassPathEngine(QStringList&& resourceEntries)
    : QAbstractFileEngine(),
      m_resourceEntries(std::move(resourceEntries))
{}

QClassPathEngine::~QClassPathEngine(){
    try{
        QClassPathEngine::close();
    } catch(const JavaException& exn) {
        if(JniEnvironment env{400}){
            exn.report(env);
        }
    } catch(...){}
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        m_engines.swap(engines);
    }
    qDeleteAll(engines);
}

QAbstractFileEngine* QClassPathEngine::getFirstEngine() const{
    QMutexLocker locker(&m_mutex);
    return m_engines.value(0);
}

void QClassPathEngine::setFileName(const QString &fileName)
{
    if (fileName==QClassPathEngine::fileName())
        return;
    QList<QAbstractFileEngine*> engines;
    auto scoped = qScopeGuard([&](){
        {
            QMutexLocker locker(&m_mutex);
            m_engines.swap(engines);
        }
        qDeleteAll(engines);
    });
    {
        QLatin1String fileNamePrefix1("classpath:");
        QLatin1String fileNamePrefix2("/:classpath:");
        QLatin1String fileNamePrefix3(":classpath:");
        QLatin1String fileNamePrefix4(":");

        QClassPathEngine::close();
        if(fileName.startsWith(fileNamePrefix1)) {
            m_prefix = fileNamePrefix1;
            m_fileName = fileName.mid(fileNamePrefix1.size());
        }else if(fileName.startsWith(fileNamePrefix2)) {
            m_prefix = fileNamePrefix2;
            m_fileName = fileName.mid(fileNamePrefix2.size());
        }else if(fileName.startsWith(fileNamePrefix3)) {
            m_prefix = fileNamePrefix3;
            m_fileName = fileName.mid(fileNamePrefix3.size());
        }else if(fileName.startsWith(fileNamePrefix4)) {
            m_prefix = fileNamePrefix4;
            m_fileName = fileName.mid(fileNamePrefix4.size());
        }else {
            return;
        }

        auto idx = m_fileName.indexOf("#");
        m_selectedSource = "*";
        if (idx == -1) {
            m_baseName = m_fileName;
        } else {
            m_baseName = m_fileName.mid(idx+1);
            m_selectedSource = m_fileName.mid(0, idx);
        }

        auto last = m_baseName.length();
        decltype(last) first = 0;

        while (first < last && m_baseName[first] == '/')
            ++first;
        if (m_baseName.endsWith("/"))
            --last;

        if (last < first)
            m_baseName = "";
        else
            m_baseName = m_baseName.mid(first, last-first).replace('\\', '/');

        if (m_selectedSource=="*") {
#ifdef Q_OS_ANDROID
            {
                QFileInfo file = QFileInfo("assets:/qrc/" + m_baseName);
                if(!file.exists()){
                    file = QFileInfo("assets:qrc/" + m_baseName);
                }
                if(file.exists()){
                    if(file.isDir()){
                        QStringList dirEntries = QDir(file.absoluteFilePath()).entryList();
                        engines << new QAssetDirectoryEngine(std::move(dirEntries), m_baseName);
                        return;
                    }else if(m_prefix==":"){
                        static AAssetManager *assetManager = nullptr;
                        {
                            QMutexLocker locker(&m_mutex);
                            if(!assetManager){
                                locker.unlock();
                                AAssetManager *_assetManager = nullptr;
                                if(JniEnvironment env{24}){
                                    try{
                                        if(jobject context = Java::Android::QtNative::getContext(env)){
                                            if(jobject assets = Java::Android::ContextWrapper::getAssets(env, context)){
                                                _assetManager = AAssetManager_fromJava(env, assets);
                                            }
                                        }
                                    } catch (const JavaException& e) {
                                        e.report(env);
                                    }
                                }
                                locker.relock();
                                assetManager = _assetManager;
                            }
                        }
                        if(assetManager){
                            engines << new QAssetEntryEngine(assetManager, m_baseName);
                            return;
                        }
                    }
                }
            }
#endif
            QString directPath;
            QSet<QString> resourcePaths = QClassPathFileEngineHandler::jarFilesByDirectory(m_baseName, &directPath);
            if(!directPath.isEmpty()){
                QFSEntryEngine* engine = new QFSEntryEngine(directPath);
                engines << engine;
            }else if (!resourcePaths.isEmpty()) { // Its at least a directory which exists in jar files
                if(JniEnvironment env{int(16*resourcePaths.size())}){
                    try{
                        //QString fileName = m_baseName + u'/' + m_fileName;
                        for(const QString& jarFilePath : qAsConst(resourcePaths)) {
                            jobject jarFile = Java::QtJambi::ResourceUtility::resolveFileToJarResource(env, qtjambi_cast<jstring>(env, jarFilePath));
                            if(jarFile){
                                std::unique_ptr<QJarEntryEngine> engine{new QJarEntryEngine(env, jarFile, jarFilePath, m_baseName, m_prefix)};
                                if(engine->isValid()) {
                                    engines << engine.release();
                                }
                            }
                        }
                    } catch (const JavaException& e) {
                        e.report(env);
                    }
                }
                if(!engines.isEmpty())
                    return;
            }
            QString filePath;
            QString topLevelDir;

            auto pos = m_baseName.lastIndexOf("/");
            if (pos >= 0)
                filePath = m_baseName.mid(0, pos);
            pos = m_baseName.indexOf("/");
            if (pos >= 0)
                topLevelDir = m_baseName.mid(0, pos);

            // This is all wrong... we need to maintain the ordered list of the mix then attempt
            //  to populate from each in turn (if we are exhaustive) otherwise
            if(filePath!=m_baseName){
                resourcePaths = QClassPathFileEngineHandler::jarFilesByDirectory(filePath);
                if (!resourcePaths.isEmpty()) {
                    if(JniEnvironment env{int(16*resourcePaths.size())}){
                        try{
                            for(const QString& jarFilePath : qAsConst(resourcePaths)) {
                                jobject jarFile = Java::QtJambi::ResourceUtility::resolveFileToJarResource(env, qtjambi_cast<jstring>(env, jarFilePath));
                                std::unique_ptr<QJarEntryEngine> engine{new QJarEntryEngine(env, jarFile, jarFilePath, m_baseName, m_prefix)};
                                bool isValid;
                                if((isValid = engine->isValid())) {
                                    engines << engine.release();
                                }
                                if(isValid){
                                    QClassPathFileEngineHandler::insertJarFileResource(jarFilePath, m_baseName);
                                }
                            }
                        } catch (const JavaException& e) {
                            e.report(env);
                        }
                    }
                }
            }
            if(!topLevelDir.isEmpty()){
                resourcePaths = QClassPathFileEngineHandler::classPathsByDirectory(topLevelDir);
                if (!resourcePaths.isEmpty()) {
                    if(JniEnvironment env{int(16*resourcePaths.size())}){
                        try{
                            for(const QString& classPath : qAsConst(resourcePaths)) {
                                QDir directory(classPath);
                                QFileInfo file(directory.absoluteFilePath(m_baseName));
                                if(file.exists()){
                                    QFSEntryEngine* engine = new QFSEntryEngine(file.absoluteFilePath());
                                    engines << engine;
                                }
                            }
                        } catch (const JavaException& e) {
                            e.report(env);
                        }
                    }
                }
            }
            resourcePaths = QClassPathFileEngineHandler::classPathDirURLs();
            if (!resourcePaths.isEmpty()) {
                if(JniEnvironment env{int(16*resourcePaths.size())}){
                    try{
                        for(const QString& path : qAsConst(resourcePaths)) {
                            jobject url = Java::QtJambi::ResourceUtility::resolveUrlFromPath(env, qtjambi_cast<jstring>(env, path));
                            addFromPath(env, url, m_baseName, m_prefix, engines);
                        }
                    } catch (const JavaException& e) {
                        e.report(env);
                    }
                }
            }
        } else if(JniEnvironment env{100}){
            try {
                jobject url = Java::QtJambi::ResourceUtility::resolveUrlFromPath(env, qtjambi_cast<jstring>(env, m_selectedSource));
                addFromPath(env, url, m_baseName, m_prefix, engines);
            } catch (const JavaException& e) {
                e.report(env);
            }
        }
    }
}

bool QClassPathEngine::copy(const QString& newName) {
    if(QAbstractFileEngine* afe = getFirstEngine())
        return afe->copy(newName);
    return false;
}

bool QClassPathEngine::setPermissions(uint perms) {
    QMutexLocker locker(&m_mutex);
    for(QAbstractFileEngine* engine : m_engines) {
        if(engine->setPermissions(perms))
            return true;
    }
    return false;
}

bool QClassPathEngine::close() {
    bool result = false;
    if(QAbstractFileEngine* afe = getFirstEngine()){
        result |= afe->close();
    }
    return result;
}

QStringList QClassPathEngine::entryList(QDir::Filters filters, const QStringList& filterNames) const {
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        engines = m_engines;
    }
    QStringList result(m_resourceEntries);
    for (QAbstractFileEngine* engine : engines) {
        result << engine->entryList(filters, filterNames);
    }
    result.removeDuplicates();
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
QStringList QClassPathEngine::entryList(QDirListing::IteratorFlags filters, const QStringList &filterNames) const{
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        engines = m_engines;
    }
    QStringList result(m_resourceEntries);
    for (QAbstractFileEngine* engine : engines) {
        result << engine->entryList(filters, filterNames);
    }
    result.removeDuplicates();
    return result;
}
#endif

QAbstractFileEngine::FileFlags QClassPathEngine::fileFlags(FileFlags type) const {
    FileFlags flags;
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        engines = m_engines;
    }

    for (QAbstractFileEngine* engine : engines)
        flags |= engine->fileFlags(type);

    if (fileName(PathName)=="/")
        flags |= QAbstractFileEngine::RootFlag;

    flags.setFlag(LocalDiskFlag, false);

    return flags;
}

QString QClassPathEngine::fileName(FileName file) const {
    QString result;
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        engines = m_engines;
    }
    if(!engines.isEmpty()){
        switch(file){
        case DefaultName:
            result = m_prefix + m_fileName;
            break;
        case CanonicalName:
            result = fileName(CanonicalPathName) + QLatin1String("/") + fileName(BaseName);
            break;
        case AbsoluteName:
#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
        case LinkName:
#endif
            result = m_prefix + m_baseName;
            break;
        case BaseName:{
                auto pos = m_baseName.lastIndexOf('/');
                result = pos > 0 ? m_baseName.mid(pos + 1) : m_baseName;
            }
            break;
        case CanonicalPathName:
        case AbsolutePathName:
        case PathName:
        default:{
                auto pos = m_baseName.lastIndexOf('/');
                result = m_prefix + (pos > 0 ? m_baseName.mid(0, pos) : "");
            }
            break;
        }
    }
    return result;
}

QDateTime QClassPathEngine::fileTime(FileTime time) const {
    if(QAbstractFileEngine* afe = getFirstEngine())
        return afe->fileTime(time);
    return QDateTime();
}

bool QClassPathEngine::link(const QString& newName) {
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        engines = m_engines;
    }
    for (QAbstractFileEngine* engine : engines) {
        if (engine->link(newName))
            return true;
    }
    return false;
}

bool QClassPathEngine::mkdir(const QString& dirName, bool createParentDirectories
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
                            ,std::optional<QFile::Permissions> permissions
#endif
                             ) const {
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        engines = m_engines;
    }
    for (QAbstractFileEngine* engine : engines) {
        if (engine->mkdir(dirName, createParentDirectories
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
                                 ,permissions
#endif
                          ))
            return true;
    }
    return false;
}

bool QClassPathEngine::open(QIODevice::OpenMode openMode
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
                    ,std::optional<QFile::Permissions> permissions
#endif
                                                 ) {
    if(QAbstractFileEngine* afe = getFirstEngine()){
        bool result = afe->open(openMode
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
                                  ,permissions
#endif
                                  );
        if(result){
            // without this, engine remains at UnspecifiedError which avoids QNetworkAccessManager to work right.
            setError(QFileDevice::NoError, "");
        }
        return result;
    }
    return false;
}

qint64 QClassPathEngine::pos() const {
    if(QAbstractFileEngine* afe = getFirstEngine())
        return afe->pos();
    return -1;
}

qint64 QClassPathEngine::read(char *data, qint64 maxlen) {
    if(QAbstractFileEngine* afe = getFirstEngine())
        return afe->read(data, maxlen);
    return -1;
}

qint64 QClassPathEngine::readLine(char *data, qint64 maxlen) {
    if(QAbstractFileEngine* afe = getFirstEngine())
        return afe->readLine(data, maxlen);
    return -1;
}

bool QClassPathEngine::remove() {
    if(QAbstractFileEngine* afe = getFirstEngine())
        return afe->remove();
    return false;
}

bool QClassPathEngine::rename(const QString& newName) {
    bool ok = true;
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        engines = m_engines;
    }
    for (QAbstractFileEngine* engine : engines)
        ok = ok && engine->rename(newName);
    return ok;
}

bool QClassPathEngine::rmdir(const QString& dirName, bool recursive) const {
    bool ok = true;
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        engines = m_engines;
    }
    for (QAbstractFileEngine* engine : engines)
        ok = ok && engine->rmdir(dirName, recursive);
    return ok;
}

bool QClassPathEngine::seek(qint64 offset) {
    if(QAbstractFileEngine* afe = getFirstEngine())
        return afe->seek(offset);
    return false;
}

QString QClassPathEngine::owner(FileOwner owner) const {
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        engines = m_engines;
    }
    for (QAbstractFileEngine* engine : engines){
        QString result = engine->owner(owner);
        if(!result.isEmpty())
            return result;
    }
    return "";
}

uint QClassPathEngine::ownerId(FileOwner owner) const {
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        engines = m_engines;
    }
    for (QAbstractFileEngine* engine : engines){
        uint result = engine->ownerId(owner);
        if (result != uint(-2))
            return result;
    }
    return uint(-2);
}

bool QClassPathEngine::isSequential() const {
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        engines = m_engines;
    }
    for (QAbstractFileEngine* engine : engines) {
        if (engine->isSequential())
            return true;
    }
    return false;
}

bool QClassPathEngine::setSize(qint64 sz) {
    if(QAbstractFileEngine* afe = getFirstEngine())
        return afe->setSize(sz);
    return false;
}

qint64 QClassPathEngine::size() const{
    if(QAbstractFileEngine* afe = getFirstEngine())
        return afe->size();
    return -1;
}

qint64 QClassPathEngine::write(const char *data, qint64 maxlen) {
    if(QAbstractFileEngine* afe = getFirstEngine())
        return afe->write(data, maxlen);
    return -1;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
QClassPathEngine::IteratorPtr QClassPathEngine::beginEntryList(const QString &path, QDirListing::IteratorFlags filters, const QStringList& nameFilters){
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        engines = m_engines;
    }
    QStringList entries(m_resourceEntries);
    for (QAbstractFileEngine* engine : engines){
        entries << engine->entryList(filters, nameFilters);
    }
    entries.removeDuplicates();
    return QClassPathEngine::IteratorPtr(new QClassPathEngineIterator(path, entries, filters, nameFilters));
}
#else
QClassPathEngine::IteratorPtr QClassPathEngine::beginEntryList(QDir::Filters filters, const QStringList& nameFilters) {
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        engines = m_engines;
    }
    QStringList entries(m_resourceEntries);
    for (QAbstractFileEngine* engine : engines){
        entries << engine->entryList(filters, nameFilters);
    }
    entries.removeDuplicates();
    return QClassPathEngine::IteratorPtr(new QClassPathEngineIterator(entries, filters, nameFilters));
}
#endif

bool QClassPathEngine::addFromPath(JNIEnv* env, jobject url, const QString& fileName, const QString& prefix, QList<QAbstractFileEngine*>& engines) {
    Q_UNUSED(prefix)
    // If it is a plain file on the disk, just read it from the disk
    if(!url)
        return false;
    QString urlPath = qtjambi_cast<QString>(env, Java::Runtime::Object::toString(env, url));
    if(urlPath.startsWith("file:")){
        QFileInfo file(QUrl(urlPath).toLocalFile());
        if (file.isDir()
#ifdef Q_OS_ANDROID
                || urlPath.endsWith("/")
#endif
                ) {
            file = QFileInfo(file.absolutePath() + "/" + fileName);
            if(file.exists()){
                QFSEntryEngine* engine = new QFSEntryEngine(file.absoluteFilePath());
                engines << engine;
                return true;
            }
            return false;
#ifdef Q_OS_ANDROID
        }else if(urlPath=="file:/"){
            file = QFileInfo("assets:/qrc/" + fileName);
            if(!file.exists()){
                file = QFileInfo("assets:qrc/" + fileName);
            }
            if(file.exists()){
                if(file.isDir()){
                    QStringList dirEntries = QDir(file.absoluteFilePath()).entryList();
                    engines << new QAssetDirectoryEngine(std::move(dirEntries), fileName);
                    return true;
                }else if(prefix==":"){
                    static AAssetManager *assetManager = nullptr;
                    {
                        QMutexLocker locker(&m_mutex);
                        if(!assetManager){
                            locker.unlock();
                            AAssetManager *_assetManager = nullptr;
                            if(jobject context = Java::Android::QtNative::getContext(env)){
                                if(jobject assets = Java::Android::ContextWrapper::getAssets(env, context)){
                                    _assetManager = AAssetManager_fromJava(env, assets);
                                }
                            }
                            locker.relock();
                            assetManager = _assetManager;
                        }
                    }
                    if(assetManager){
                        engines << new QAssetEntryEngine(assetManager, fileName);
                        return true;
                    }
                }
            }
#endif
        }else if(file.isFile()){
            urlPath = "jar:" + urlPath + "!/";
            url = Java::QtJambi::ResourceUtility::resolveUrlFromPath(env, qtjambi_cast<jstring>(env, urlPath));
        }
    }else if(urlPath.endsWith(".jar") || urlPath.endsWith(".apk") || urlPath.endsWith(".aab")){
        urlPath = "jar:" + urlPath + "!/";
        url = Java::QtJambi::ResourceUtility::resolveUrlFromPath(env, qtjambi_cast<jstring>(env, urlPath));
    }
    urlPath = urlPath + fileName;
    url = Java::QtJambi::ResourceUtility::resolveUrlFromPath(env, qtjambi_cast<jstring>(env, urlPath));
    if(url){
        std::unique_ptr<QUrlEntryEngine> engine{new QUrlEntryEngine(env, url, fileName)};
        if(engine->fileFlags(QAbstractFileEngine::FileInfoAll).testFlag(QAbstractFileEngine::ExistsFlag)) {
            engines << engine.release();
            return true;
        }
    }
    return false;
}

QUrlEntryEngine::QUrlEntryEngine(JNIEnv* env, jobject url, const QString& fileName)
    : QAbstractFileEngine(),
      QClassPathEntry(),
      m_fileName(fileName),
      m_connection(),
      m_readChannel(),
      m_size(-1),
      m_flags()
{
    try{
        jobject connection = Java::Runtime::URL::openConnection(env, url);
        if(connection){
            m_connection = {env, connection};
            m_size = Java::Runtime::URLConnection::getContentLengthLong(env, connection);
            m_flags.setFlag(ExistsFlag);
            if(Java::Runtime::JarURLConnection::isInstanceOf(env, connection)
                && Java::Runtime::ZipEntry::isDirectory(env, Java::Runtime::JarURLConnection::getJarEntry(env, connection))){
                m_flags.setFlag(DirectoryType);
            }else if(m_size==0){
                m_flags.setFlag(DirectoryType);
            }else{
                m_flags.setFlag(FileType);
            }
            if(Java::Runtime::URLConnection::getDoInput(env, connection)){
                m_flags |= ReadGroupPerm | ReadOtherPerm | ReadOwnerPerm | ReadUserPerm;
            }
        }
    }catch(const JavaException& exn){
        exn.report(env);
    }
}

QUrlEntryEngine::~QUrlEntryEngine(){
    QUrlEntryEngine::close();
}

qint64 QUrlEntryEngine::read(char *data, qint64 maxlen) {
    if(m_readChannel){
        if(JniEnvironment env{400}){
            try{
                jobject buffer = env->NewDirectByteBuffer(data, maxlen);
                Java::Runtime::ReadableByteChannel::read(env, m_readChannel.object(env), buffer);
                jint pos = Java::Runtime::Internal::Buffer::position(env, buffer);
                truncateBuffer(env, buffer);
                return pos;
            }catch(const JavaException& exn){
                exn.report(env);
                return 0;
            }
        }
    }
    return 0;
}

bool QUrlEntryEngine::open(QIODevice::OpenMode openMode
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
         ,std::optional<QFile::Permissions>
#endif
        ) {
    if((openMode & QIODevice::ReadOnly) && !m_readChannel && !m_flags.testFlag(DirectoryType)){
        if(JniEnvironment env{400}){
            try{
                if(jobject stream = Java::Runtime::URLConnection::getInputStream(env, m_connection.object(env))){
                    m_readChannel.assign(env, Java::Runtime::Channels::newInChannel(env, stream));
                }else{
                    m_readChannel.assign(env, nullptr);
                }
            }catch(const JavaException& exn){
                exn.report(env);
                return false;
            }
        }
        return true;
    }
    return false;
}

bool QUrlEntryEngine::close() {
    if(m_readChannel){
        if(JniEnvironment env{400}){
            try{
                Java::Runtime::AutoCloseable::close(env, m_readChannel.object(env));
                m_readChannel.assign(env, nullptr);
                return true;
            }catch(const JavaException& exn){
                exn.report(env);
            }
        }
    }
    return false;
}

QAbstractFileEngine::FileFlags QUrlEntryEngine::fileFlags(FileFlags f) const {
    return m_flags & f;
}

class AccessResource : public QResource{
public:
    static bool needsClassPathEngine(const QString &fileName, QStringList& resourceEntries);
};

bool AccessResource::needsClassPathEngine(const QString &fileName, QStringList& resourceEntries){
    AccessResource resource;
    resource.setFileName(fileName);
    if(resource.isValid()){
        resourceEntries << resource.children();
        return !resourceEntries.isEmpty();
    }
    return true;
}

QClassPathFileEngineHandler::QAbstractFileEnginePointer QClassPathFileEngineHandler::create(const QString &fileName) const
{
    QClassPathEngine *rv = nullptr;
    bool mightBeResource = false;
    if (fileName.startsWith("classpath:")
            || fileName.startsWith("/:classpath:")
            || fileName.startsWith(":classpath:")
            || (mightBeResource = fileName.startsWith(":"))){
        QStringList resourceEntries;
        if(!mightBeResource || AccessResource::needsClassPathEngine(fileName, resourceEntries)){
            rv = new QClassPathEngine(std::move(resourceEntries));
            rv->setFileName(fileName);
            if(!rv->isValid()){
                delete rv;
                rv = nullptr;
            }
        }
    }
    return QAbstractFileEnginePointer(rv);
}
