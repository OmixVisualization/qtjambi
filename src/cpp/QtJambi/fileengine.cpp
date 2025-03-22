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
    static QSet<QString> classPathURLs();
private:
    mutable QReadWriteLock m_fileEngineLock;
    PathsToJarFilesHash m_jarFilesByContentDirectories;
    PathsToDirectoryHash m_filePathsByContentDirectories;
    ClassPathDirURLSet m_classPathPlainURLSet;
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
    m_classPathPlainURLSet.clear();
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
            handler->m_classPathPlainURLSet.remove(path);
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

QSet<QString> QClassPathFileEngineHandler::classPathURLs(){
    if(Q_UNLIKELY(!gClassPathFileEngineHandler.isDestroyed())){
        const QClassPathFileEngineHandler* handler = gClassPathFileEngineHandler->get();
        if(Q_LIKELY(handler)){
            QReadLocker locker(&handler->m_fileEngineLock);
            return handler->m_classPathPlainURLSet;
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
                const QFileInfoList fil = directory.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
                for(const QFileInfo& file : fil){
                    if(file.isDir()){
                        classPathDirSet[file.fileName()].insert(path);
                    }
                }
                classPathDirSet[QString()].insert(path);
            }
            QWriteLocker locker(&handler->m_fileEngineLock);
            for(auto iter = classPathDirSet.keyValueBegin(); iter!=classPathDirSet.keyValueEnd(); ++iter){
                handler->m_filePathsByContentDirectories[iter->first].unite(iter->second);
            }
            if(!isDirectory)
                handler->m_classPathPlainURLSet.insert(path);
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

class QJarEntryEngine final : public QAbstractFileEngine {
public:
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 1)
    typedef QFileDevice::FileTime FileTime;
    static constexpr FileTime BirthTime = FileTime::FileBirthTime;
    static constexpr FileTime AccessTime = FileTime::FileAccessTime;
    static constexpr FileTime MetadataChangeTime = FileTime::FileMetadataChangeTime;
    static constexpr FileTime ModificationTime = FileTime::FileModificationTime;
#endif
    static QJarEntryEngine* create(JNIEnv* env, const QString& jarFileName, const QString& fileName, const QString& prefix);
    QJarEntryEngine(JNIEnv* env, jobject myJarFile, const QString& jarFileName, const QString& name, const QString& prefix);
    QJarEntryEngine(JNIEnv* env, jobject myJarFile, const QString& jarFileName, jobject entry, QString&& name, bool isDirectory, qint64 size, const QString& prefix);
    ~QJarEntryEngine() override;

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

    QDateTime fileTime(FileTime time) const override;

    bool open(QIODevice::OpenMode openMode
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
             ,std::optional<QFile::Permissions> permissions = std::nullopt
#endif
              ) override;

    qint64 pos() const override;

    qint64 read(char *data, qint64 maxlen) override;
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
    QPair<const void*,qint64> m_bufferInfo;
    JObjectWrapper m_buffer;
    qint64 m_pos;
    qint64 m_size;
    QIODevice::OpenMode m_openMode;
    FileFlags m_flags;
    QString m_prefix;
};

QJarEntryEngine* QJarEntryEngine::create(JNIEnv* env, const QString& jarFileName, const QString& fileName, const QString& prefix){
    jobject jarFile = Java::QtJambi::ResourceUtility::resolveFileToJarResource(env, qtjambi_cast<jstring>(env, jarFileName));
    if(jarFile){
        if (fileName.isEmpty()) {
            return new QJarEntryEngine(env, jarFile, jarFileName, fileName, prefix);
        }

        jstring entryFileName = qtjambi_cast<jstring>(env, fileName);
        jobject entry = Java::QtJambi::ResourceUtility$JarResource::getJarEntry(env, jarFile, entryFileName);
        if (!entry) {
            if(Java::QtJambi::ResourceUtility$JarResource::isDirectory(env, jarFile, entryFileName))
                return new QJarEntryEngine(env, jarFile, jarFileName, fileName, prefix);
        } else {
            bool directory = Java::QtJambi::ResourceUtility$JarResource::checkIsDirectory(env, jarFile, entry);
            qint64 size = directory ? 0 : Java::Runtime::ZipEntry::getSize(env, entry);
            return new QJarEntryEngine(env, jarFile, jarFileName, entry,
                                       qtjambi_cast<QString>(env, Java::Runtime::ZipEntry::getName(env, entry)),
                                       directory, size, prefix);
        }
    }
    return nullptr;
}

QJarEntryEngine::QJarEntryEngine(JNIEnv* env, jobject myJarFile, const QString& jarFileName, const QString& name, const QString& prefix)
  : QAbstractFileEngine(),
    m_jarFileName(jarFileName),
    m_entryFileName(name),
    m_entry(),
    m_myJarFile(env, myJarFile),
    m_readChannel(),
    m_bufferInfo{nullptr,0},
    m_buffer(),
    m_pos(-1),
    m_size(0),
    m_openMode(QIODevice::NotOpen),
    m_flags(ExistsFlag | DirectoryType),
    m_prefix(prefix)
{

}

QJarEntryEngine::QJarEntryEngine(JNIEnv* env, jobject myJarFile, const QString& jarFileName, jobject entry, QString&& name, bool isDirectory, qint64 size, const QString& prefix)
    : QAbstractFileEngine(),
      m_jarFileName(jarFileName),
      m_entryFileName(std::move(name)),
      m_entry(env, entry),
      m_myJarFile(env, myJarFile),
      m_readChannel(),
      m_bufferInfo{nullptr,0},
      m_buffer(),
      m_pos(-1),
      m_size(size),

      m_openMode(QIODevice::NotOpen),
      m_flags(ReadUserPerm | ReadOwnerPerm | ReadGroupPerm | ReadOtherPerm | ExistsFlag | (isDirectory ? DirectoryType : FileType)),
      m_prefix(prefix)
{
}

QJarEntryEngine::~QJarEntryEngine(){
    if(m_readChannel || m_openMode.testFlag(QIODevice::ReadOnly)){
        if(JniEnvironment env{400}){
            try {
                QJarEntryEngine::close(env);
            } catch(const JavaException& exn) {
                exn.report(env);
            } catch(...){}
            m_entry.assign(env, nullptr);
            m_readChannel.assign(env, nullptr);
            m_buffer.assign(env, nullptr);
            m_myJarFile.assign(env, nullptr);
        }
    }
}

bool QJarEntryEngine::close(){
    bool result = false;
    if(m_readChannel || m_openMode.testFlag(QIODevice::ReadOnly)) {
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
    QIODevice::OpenMode openMode = m_openMode;
    bool bf = closeStream(env);

    if(openMode.testFlag(QIODevice::ReadOnly)) {
        Java::QtJambi::ResourceUtility$JarResource::_deref(env, m_myJarFile.object(env));
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
        if(m_buffer){
            truncateBuffer(env, m_buffer);
            m_buffer.assign(env, nullptr);
        }
        try{
            Java::QtJambi::ResourceUtility$JarResource::_deref(env, m_myJarFile.object(env));
        }catch(const JavaException&){
        }
        return true;
    }
    m_openMode = QIODevice::NotOpen;
    return false;
}

QStringList QJarEntryEngine::entryList(QDir::Filters filters, const QStringList &filterNames) const{
    QStringList result;
    if (m_flags.testFlag(DirectoryType)){
        if (filters & QDir::Dirs) {
            if(!(filters & QDir::NoDot))
                result << ".";
            if (m_entryFileName.length() > 0 && !(filters & QDir::NoDotDot))
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
                                               qtjambi_cast<jstring>(env, m_entryFileName), false);
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
    if (m_flags.testFlag(DirectoryType)){
        if(!(filters & QDirListing::IteratorFlag::FilesOnly)
            && (filters & QDirListing::IteratorFlag::IncludeDotAndDotDot)){
            result << ".";
            if(m_entryFileName.length() > 0)
                result << "..";
        }
        if(JniEnvironmentExceptionHandler env{600}){
            try{
                Java::QtJambi::ResourceUtility$JarResource::entryList(env, m_myJarFile.object(env),
                                                                  qtjambi_cast<jobject>(env, &result),
                                                                  jint(int(filters)),
                                                                  qtjambi_cast<jobject>(env, filterNames),
                                                                  qtjambi_cast<jstring>(env, m_entryFileName), true);
            }catch(const JavaException& exn){
                env.handleException(exn, this, "QJarEntryEngine::entryList(QDirListing::IteratorFlags, QStringList)const");
            }
        }
    }
    return result;
}
#endif


QAbstractFileEngine::FileFlags QJarEntryEngine::fileFlags(FileFlags type) const{
    return m_flags & type;
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
        if (!openMode.testFlag(QIODevice::WriteOnly) && !openMode.testFlag(QIODevice::Append) && openMode.testFlag(QIODevice::ReadOnly)) {
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
                        bf = true;
                    }else{
                        setError(QFile::OpenError, QStringLiteral(u"Onable to open jar entry %s").arg(this->m_entryFileName));
                        if(oldRefCount >= 0) {
                            if(oldRefCount == 0)
                                Java::QtJambi::ResourceUtility$JarResource::_deref(env, m_myJarFile.object(env)); // rollback extra reference
                            Java::QtJambi::ResourceUtility$JarResource::_deref(env, m_myJarFile.object(env)); // rollback reference
                        }
                    }
                }catch(const JavaException& exn){
                    setError(QFile::OpenError, QString::asprintf("Exception in file open: %s", exn.what()));
                    env.handleException(exn, this, "QJarEntryEngine::open(QIODevice::OpenMode)");
                }
            }
        }else{
            setError(QFile::OpenError, QStringLiteral(u"Onable to open jar entry %s for writing").arg(this->m_entryFileName));
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
            try{
                jobject buffer{nullptr};
                if(m_pos==0 && m_size<=maxlen){
                    if(m_buffer){
                        m_buffer.assign(env, nullptr);
                        m_bufferInfo.first = nullptr;
                        m_bufferInfo.second = 0;
                    }
                    buffer = env->NewDirectByteBuffer(data, maxlen);
                    Java::Runtime::ReadableByteChannel::read(env, m_readChannel.object(env), buffer);
                    readBytes = Java::Runtime::Internal::Buffer::position(env, buffer);
                    truncateBuffer(env, buffer);
                }else{
                    if(m_buffer.isNull() || m_bufferInfo.first!=data || m_bufferInfo.second!=maxlen){
                        if(m_buffer)
                            truncateBuffer(env, m_buffer.object(env));
                        m_buffer.assign(env, buffer = env->NewDirectByteBuffer(data, maxlen));
                        m_bufferInfo.first = data;
                        m_bufferInfo.second = maxlen;
                    }else{
                        buffer = m_buffer.object(env);
                        Java::Runtime::Internal::Buffer::clear(env, buffer);
                    }
                    Java::Runtime::ReadableByteChannel::read(env, m_readChannel.object(env), buffer);
                    readBytes = Java::Runtime::Internal::Buffer::position(env, buffer);
                    if(readBytes==0){
                        truncateBuffer(env, buffer);
                        m_buffer.assign(env, nullptr);
                        m_bufferInfo.first = nullptr;
                        m_bufferInfo.second = 0;
                    }else{
                        Java::Runtime::Internal::Buffer::setLimit(env, buffer, 0);
                    }
                }
            }catch(const JavaException& exn){
                setError(QFile::ReadError, QString::asprintf("Exception in read: %s", exn.what()));
                env.handleException(exn, this, "QJarEntryEngine::read(char*,qint64)");
            }
        }else {
            setError(QFile::ReadError, "Unable to read without JavaVM");
        }
    }else {
        setError(QFile::ReadError, "File not open");
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
class QAssetEntryEngine final : public QAbstractFileEngine {
public:
    QAssetEntryEngine(AAssetManager *assetManager, const QString& fileName);
    ~QAssetEntryEngine() override;

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
      m_fileName(fileName),
      m_assetManager(assetManager)
{
    m_fileName.replace(QLatin1String("//"), QLatin1String("/"));
    if (m_fileName.startsWith(QLatin1Char('/')))
        m_fileName.remove(0, 1);
    if (m_fileName.endsWith(QLatin1Char('/')))
        m_fileName.chop(1);
    open(QIODevice::ReadOnly);
}

QAssetEntryEngine::~QAssetEntryEngine(){
    QAssetEntryEngine::close();
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


class QAssetDirectoryEngine final : public QAbstractFileEngine {
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

    bool isValid() const { return true; }
private:
    QString m_fileName;
    QStringList m_dirEntries;
};

QAssetDirectoryEngine::QAssetDirectoryEngine(QStringList&& dirEntries, const QString& fileName)
    : QAbstractFileEngine(),
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
#endif //def Q_OS_ANDROID

class QUrlEntryEngine final : public QAbstractFileEngine {
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

    bool seek(qint64 offset) override;

    qint64 size() const override{
        return m_size;
    }

    bool caseSensitive() const override {
        return true;
    }

    FileFlags fileFlags(FileFlags) const override;
    qint64 pos() const override;
private:
    bool reset();
    bool reopen();
    bool closeStream();
    bool closeStream(JNIEnv* env);
    QString m_fileName;
    JObjectWrapper m_connection;
    JObjectWrapper m_readChannel;
    QPair<const void*,qint64> m_bufferInfo;
    JObjectWrapper m_buffer;
    qint64 m_size;
    qint64 m_pos;
    QIODevice::OpenMode m_openMode;
    FileFlags m_flags;
};

QUrlEntryEngine::QUrlEntryEngine(JNIEnv* env, jobject url, const QString& fileName)
    : QAbstractFileEngine(),
    m_fileName(fileName),
    m_connection(),
    m_readChannel(),
    m_bufferInfo{nullptr,0},
    m_buffer(),
    m_size(0),
    m_pos(-1),
    m_openMode(QIODevice::NotOpen),
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
    QUrlEntryEngine::closeStream();
}

qint64 QUrlEntryEngine::pos() const {
    return m_pos;
}

qint64 QUrlEntryEngine::read(char *data, qint64 maxlen) {
    qint64 readBytes = 0;
    if(m_readChannel){
        if(JniEnvironmentExceptionHandler env{600}){
            try{
                jobject buffer{nullptr};
                if(m_pos==0 && m_size<=maxlen){
                    if(m_buffer){
                        m_buffer.assign(env, nullptr);
                        m_bufferInfo.first = nullptr;
                        m_bufferInfo.second = 0;
                    }
                    buffer = env->NewDirectByteBuffer(data, maxlen);
                    Java::Runtime::ReadableByteChannel::read(env, m_readChannel.object(env), buffer);
                    readBytes = Java::Runtime::Internal::Buffer::position(env, buffer);
                    truncateBuffer(env, buffer);
                }else{
                    if(m_buffer.isNull() || m_bufferInfo.first!=data || m_bufferInfo.second!=maxlen){
                        if(m_buffer)
                            truncateBuffer(env, m_buffer.object(env));
                        m_buffer.assign(env, buffer = env->NewDirectByteBuffer(data, maxlen));
                        m_bufferInfo.first = data;
                        m_bufferInfo.second = maxlen;
                    }else{
                        buffer = m_buffer.object(env);
                        Java::Runtime::Internal::Buffer::clear(env, buffer);
                    }
                    Java::Runtime::ReadableByteChannel::read(env, m_readChannel.object(env), buffer);
                    readBytes = Java::Runtime::Internal::Buffer::position(env, buffer);
                    if(readBytes==0){
                        truncateBuffer(env, buffer);
                        m_buffer.assign(env, nullptr);
                        m_bufferInfo.first = nullptr;
                        m_bufferInfo.second = 0;
                    }else{
                        Java::Runtime::Internal::Buffer::setLimit(env, buffer, 0);
                    }
                }
            }catch(const JavaException& exn){
                setError(QFile::ReadError, QString::asprintf("Exception in read: %s", exn.what()));
                env.handleException(exn, this, "QUrlEntryEngine::read(char*,qint64)");
            }
        }else{
            setError(QFile::ReadError, "Unable to read without JavaVM");
        }
    }else {
        setError(QFile::ReadError, "File not open");
    }
    if(readBytes==0)
        return -1;
    m_pos += readBytes;
    return readBytes;
}

bool QUrlEntryEngine::open(QIODevice::OpenMode openMode
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
                           ,std::optional<QFile::Permissions>
#endif
                           ) {
    bool bf{false};
    if (!openMode.testFlag(QIODevice::WriteOnly) && !openMode.testFlag(QIODevice::Append) && openMode.testFlag(QIODevice::ReadOnly) && !m_flags.testFlag(DirectoryType)) {
        if(JniEnvironmentExceptionHandler env{600}){
            try{
                closeStream(env);
                if(jobject stream = Java::Runtime::URLConnection::getInputStream(env, m_connection.object(env))){
                    m_readChannel.assign(env, Java::Runtime::Channels::newInChannel(env, stream));
                    m_pos = 0;
                    m_openMode = openMode;
                    bf = true;
                }else{
                    setError(QFile::OpenError, QStringLiteral(u"Unable to open url %s").arg(this->m_fileName));
                    m_readChannel.assign(env, nullptr);
                }
            }catch(const JavaException& exn){
                setError(QFile::OpenError, QString::asprintf("Exception in url open: %s", exn.what()));
                env.handleException(exn, this, "QUrlEntryEngine::open(QIODevice::OpenMode)");
            }
        }
    }else{
        setError(QFile::OpenError, QStringLiteral(u"Unable to open url %s for writing").arg(this->m_fileName));
        closeStream();
    }
    return bf;
}

bool QUrlEntryEngine::seek(qint64 offset) {
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

bool QUrlEntryEngine::reset() {
    if (!m_readChannel)
        return false;  // not open
    if (m_pos == 0)
        return true;  // already open and at start
    return reopen();
}

bool QUrlEntryEngine::reopen() {
    if (!m_readChannel)
        return false;  // not open
    QIODevice::OpenMode om = m_openMode;  // saved OpenMode
    if(closeStream())
        return open(om);
    return false;
}

bool QUrlEntryEngine::closeStream() {
    if(m_readChannel || m_openMode.testFlag(QIODevice::ReadOnly)) {
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

bool QUrlEntryEngine::closeStream(JNIEnv* env) {
    if(m_readChannel || m_openMode.testFlag(QIODevice::ReadOnly)) {
        try {
            Java::Runtime::AutoCloseable::close(env, m_readChannel.object(env));
        } catch(const JavaException&) {
        }
        m_readChannel.assign(env, nullptr);
        if(m_buffer){
            truncateBuffer(env, m_buffer);
            m_buffer.assign(env, nullptr);
        }
        return true;
    }
    m_openMode = QIODevice::NotOpen;
    return false;
}

bool QUrlEntryEngine::close() {
    return closeStream();
}

QAbstractFileEngine::FileFlags QUrlEntryEngine::fileFlags(FileFlags f) const {
    return m_flags & f;
}

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
    static QLatin1String fileNamePrefix1;
    static QLatin1String fileNamePrefix2;
    static QLatin1String fileNamePrefix3;
    static QLatin1String fileNamePrefix4;

    ~QClassPathEngine() override;

    void setFileName(const QString &fileName) override;

    bool copy(const QString& newName) override;

    bool setPermissions(uint perms) override;

    bool caseSensitive() const override;

    bool close() override;

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

    qint64 readLine(char *data, qint64 maxlen) override;

    bool seek(qint64 offset) override;

    QString owner(FileOwner owner) const override;

    uint ownerId(FileOwner owner) const override;

    bool isRelativePath() const override {
        return false;
    }

    bool isSequential() const override;

    qint64 size() const override;

    QStringList entryList(QDir::Filters filters, const QStringList& filterNames) const override;

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    QStringList entryList(QDirListing::IteratorFlags filters, const QStringList& filterNames) const override;
    IteratorPtr beginEntryList(const QString &path, QDirListing::IteratorFlags filters, const QStringList& nameFilters) override;
#else
    IteratorPtr beginEntryList(QDir::Filters filters, const QStringList& nameFilters) override;
#endif

    bool isValid(){return !m_engines.isEmpty();}
private:
    static void initialize(QLatin1String prefix,
                           QString &fileName,
                           QString &baseName,
                           QString &selectedSource,
                           QList<QAbstractFileEngine*> &engines,
                           const QString &givenFileName);
    static bool checkAndSetPrefix(const QString & givenFileName, QLatin1String fileNamePrefix, QLatin1String& prefix);
    QClassPathEngine(QLatin1String prefix,
                     QString &&fileName,
                     QString &&baseName,
                     QString &&selectedSource,
                     QList<QAbstractFileEngine*> &&engines,
                     QStringList&& resourceEntries);
    QAbstractFileEngine* getFirstEngine() const;

    QLatin1String m_prefix;
    QString m_fileName;
    QString m_baseName;
    QString m_selectedSource;
    QList<QAbstractFileEngine*> m_engines;
    const QStringList m_resourceEntries;
    mutable QMutex m_mutex;
    friend QClassPathFileEngineHandler;
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

QLatin1String QClassPathEngine::fileNamePrefix1("classpath:");
QLatin1String QClassPathEngine::fileNamePrefix2("/:classpath:");
QLatin1String QClassPathEngine::fileNamePrefix3(":classpath:");
QLatin1String QClassPathEngine::fileNamePrefix4(":");

QClassPathEngine::QClassPathEngine(QLatin1String prefix,
                                   QString &&fileName,
                                   QString &&baseName,
                                   QString &&selectedSource,
                                   QList<QAbstractFileEngine*> &&engines,
                                   QStringList&& resourceEntries)
    : QAbstractFileEngine(),
    m_prefix(prefix),
    m_fileName(std::move(fileName)),
    m_baseName(std::move(baseName)),
    m_selectedSource(std::move(selectedSource)),
    m_engines(std::move(engines)),
    m_resourceEntries(std::move(resourceEntries))
{

}

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

bool QClassPathEngine::checkAndSetPrefix(const QString & givenFileName, QLatin1String fileNamePrefix, QLatin1String& prefix){
    if(givenFileName.startsWith(fileNamePrefix)){
        prefix = fileNamePrefix;
        return true;
    }else{
        return false;
    }
}

#ifdef Q_OS_ANDROID
AAssetManager* getAssetManager(){
    static AAssetManager *assetManager = []() -> AAssetManager* {
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
        return _assetManager;
    }();
    return assetManager;
}
#endif

void QClassPathEngine::initialize(QLatin1String prefix,
                                  QString &fileName,
                                  QString &baseName,
                                  QString &selectedSource,
                                  QList<QAbstractFileEngine*> &engines,
                                  const QString &givenFileName){
    QString slash = QStringLiteral(u"/");
    QString asterisk = QStringLiteral(u"*");
    fileName = givenFileName.mid(prefix.size());
    auto idx = fileName.indexOf(QStringLiteral(u"#"));
    selectedSource = asterisk;
    if (idx == -1) {
        baseName = fileName;
    } else {
        baseName = fileName.mid(idx+1);
        selectedSource = fileName.mid(0, idx);
    }

    auto last = baseName.length();
    decltype(last) first = 0;

    while (first < last && baseName[first] == '/')
        ++first;
    if (baseName.endsWith(slash))
        --last;

    if (last < first)
        baseName.clear();
    else
        baseName = baseName.mid(first, last-first).replace('\\', '/');

    if (selectedSource==asterisk) {
#ifdef Q_OS_ANDROID
        {
            QFileInfo file = QFileInfo(QStringLiteral(u"assets:/qrc/") + baseName);
            if(!file.exists()){
                file = QFileInfo(QStringLiteral(u"assets:qrc/") + baseName);
            }
            if(file.exists()){
                if(file.isDir()){
                    QStringList dirEntries = QDir(file.absoluteFilePath()).entryList();
                    engines << new QAssetDirectoryEngine(std::move(dirEntries), baseName);
                    return;
                }else if(prefix==fileNamePrefix1){
                    if(AAssetManager *assetManager = getAssetManager()){
                        engines << new QAssetEntryEngine(assetManager, baseName);
                        return;
                    }
                }
            }
        }
#endif
        QString directPath;
        QSet<QString> resourcePaths = QClassPathFileEngineHandler::jarFilesByDirectory(baseName, &directPath);
        if(!directPath.isEmpty()){
            engines << new QFSFileEngine(directPath);
        }else if (!resourcePaths.isEmpty()) { // Its at least a directory which exists in jar files
            if(JniEnvironment env{int(16*resourcePaths.size())}){
                try{
                    for(const QString& jarFilePath : qAsConst(resourcePaths)) {
                        if(QJarEntryEngine* engine = QJarEntryEngine::create(env, jarFilePath, baseName, prefix)) {
                            engines << engine;
                        }
                    }
                } catch (const JavaException& e) {
                    e.report(env);
                }
            }
        }
        QString filePath;
        QString topLevelDir;

        auto pos = baseName.lastIndexOf(slash);
        if (pos >= 0)
            filePath = baseName.mid(0, pos);
        pos = baseName.indexOf(slash);
        if (pos >= 0)
            topLevelDir = baseName.mid(0, pos);

        // This is all wrong... we need to maintain the ordered list of the mix then attempt
        //  to populate from each in turn (if we are exhaustive) otherwise
        if(filePath!=baseName){
            resourcePaths = QClassPathFileEngineHandler::jarFilesByDirectory(filePath);
            if (!resourcePaths.isEmpty()) {
                if(JniEnvironment env{int(16*resourcePaths.size())}){
                    try{
                        for(const QString& jarFilePath : qAsConst(resourcePaths)) {
                            if(QJarEntryEngine* engine = QJarEntryEngine::create(env, jarFilePath, baseName, prefix)) {
                                engines << engine;
                                QClassPathFileEngineHandler::insertJarFileResource(jarFilePath, baseName);
                            }
                        }
                    } catch (const JavaException& e) {
                        e.report(env);
                    }
                }
            }
        }
        resourcePaths = QClassPathFileEngineHandler::classPathsByDirectory(topLevelDir);
        if (!resourcePaths.isEmpty()) {
            if(JniEnvironment env{int(16*resourcePaths.size())}){
                try{
                    for(const QString& classPath : qAsConst(resourcePaths)) {
                        QDir directory(classPath);
                        QFileInfo file(directory.absoluteFilePath(baseName));
                        if(file.exists()){
                            engines << new QFSFileEngine(file.absoluteFilePath());
                        }
                    }
                } catch (const JavaException& e) {
                    e.report(env);
                }
            }
        }
        resourcePaths = QClassPathFileEngineHandler::classPathURLs();
        if (!resourcePaths.isEmpty()) {
            if(JniEnvironment env{int(16*resourcePaths.size())}){
                try{
                    for(const QString& path : qAsConst(resourcePaths)) {
                        if(jobject url = Java::QtJambi::ResourceUtility::resolveUrlFromPath(env, qtjambi_cast<jstring>(env, path+baseName))){
                            std::unique_ptr<QUrlEntryEngine> engine{new QUrlEntryEngine(env, url, givenFileName)};
                            if(engine->fileFlags(QAbstractFileEngine::FileInfoAll).testFlag(QAbstractFileEngine::ExistsFlag)) {
                                engines << engine.release();
                            }
                        }
                    }
                } catch (const JavaException& e) {
                    e.report(env);
                }
            }
        }
    } else if(JniEnvironment env{100}){
        if(selectedSource.startsWith(QStringLiteral(u"file:"))){
            QFileInfo file(QUrl(selectedSource).toLocalFile());
            if (file.isDir()
#ifdef Q_OS_ANDROID
                || selectedSource.endsWith(slash)
#endif
                ) {
                file = QFileInfo(file.absolutePath() + slash + baseName);
                if(file.exists()){
                    engines << new QFSFileEngine(file.absoluteFilePath());
                }
#ifdef Q_OS_ANDROID
            }else if(selectedSource==QStringLiteral(u"file:/")){
                file = QFileInfo(QStringLiteral(u"assets:/qrc/") + baseName);
                if(!file.exists()){
                    file = QFileInfo(QStringLiteral(u"assets:qrc/") + baseName);
                }
                if(file.exists()){
                    if(file.isDir()){
                        QStringList dirEntries = QDir(file.absoluteFilePath()).entryList();
                        engines << new QAssetDirectoryEngine(std::move(dirEntries), baseName);
                    }else if(prefix==fileNamePrefix1){
                        if(AAssetManager *assetManager = getAssetManager()){
                            engines << new QAssetEntryEngine(assetManager, baseName);
                        }
                    }
                }
#endif
            }else if(file.isFile()){
                try{
                    if(jobject url = Java::QtJambi::ResourceUtility::resolveUrlFromPath(env, qtjambi_cast<jstring>(env, QStringLiteral(u"jar:")+selectedSource+QStringLiteral(u"!/")+baseName))){
                        std::unique_ptr<QUrlEntryEngine> engine{new QUrlEntryEngine(env, url, givenFileName)};
                        if(engine->fileFlags(QAbstractFileEngine::FileInfoAll).testFlag(QAbstractFileEngine::ExistsFlag)) {
                            engines << engine.release();
                        }
                    }
                } catch (const JavaException& e) {
                    e.report(env);
                }
            }
#ifdef Q_OS_ANDROID
        }else if(selectedSource==QStringLiteral(u"assets:/")){
            QFileInfo file(selectedSource + slash + baseName);
            if(file.exists()){
                if(file.isDir()){
                    QStringList dirEntries = QDir(file.absoluteFilePath()).entryList();
                    engines << new QAssetDirectoryEngine(std::move(dirEntries), baseName);
                }else if(prefix==fileNamePrefix1){
                    if(AAssetManager *assetManager = getAssetManager()){
                        engines << new QAssetEntryEngine(assetManager, baseName);
                    }
                }
            }
#endif
        }else{
            QFileInfo file(selectedSource);
            if(file.exists()){
                if (file.isDir()
#ifdef Q_OS_ANDROID
                    || selectedSource.endsWith(slash)
#endif
                    ) {
                    file = QFileInfo(file.absolutePath() + slash + baseName);
                    if(file.exists()){
                        engines << new QFSFileEngine(file.absoluteFilePath());
                    }
#ifdef Q_OS_ANDROID
                }else if(selectedSource==QStringLiteral(u"file:/")){
                    file = QFileInfo(QStringLiteral(u"assets:/qrc/") + baseName);
                    if(!file.exists()){
                        file = QFileInfo(QStringLiteral(u"assets:qrc/") + baseName);
                    }
                    if(file.exists()){
                        if(file.isDir()){
                            QStringList dirEntries = QDir(file.absoluteFilePath()).entryList();
                            engines << new QAssetDirectoryEngine(std::move(dirEntries), baseName);
                        }else if(prefix==fileNamePrefix1){
                            if(AAssetManager *assetManager = getAssetManager()){
                                engines << new QAssetEntryEngine(assetManager, baseName);
                            }
                        }
                    }
#endif
                }
                return;
            }
            try{
                if(jobject url = Java::QtJambi::ResourceUtility::resolveUrlFromPath(env, qtjambi_cast<jstring>(env, selectedSource+baseName))){
                    std::unique_ptr<QUrlEntryEngine> engine{new QUrlEntryEngine(env, url, givenFileName)};
                    if(engine->fileFlags(QAbstractFileEngine::FileInfoAll).testFlag(QAbstractFileEngine::ExistsFlag)) {
                        engines << engine.release();
                    }
                }
            } catch (const JavaException& e) {
                e.report(env);
            }
        }
    }
}

void QClassPathEngine::setFileName(const QString &givenFileName)
{
    if (givenFileName==QClassPathEngine::fileName())
        return;
    if (checkAndSetPrefix(givenFileName, QClassPathEngine::fileNamePrefix1, m_prefix)
        || checkAndSetPrefix(givenFileName, QClassPathEngine::fileNamePrefix2, m_prefix)
        || checkAndSetPrefix(givenFileName, QClassPathEngine::fileNamePrefix3, m_prefix)
        || checkAndSetPrefix(givenFileName, QClassPathEngine::fileNamePrefix4, m_prefix)){
        QList<QAbstractFileEngine*> engines;
        initialize(m_prefix, m_fileName, m_baseName, m_selectedSource, engines, givenFileName);
        try{
            QClassPathEngine::close();
        } catch(const JavaException& exn) {
            if(JniEnvironment env{400}){
                exn.report(env);
            }
        } catch(...){}
        {
            QMutexLocker locker(&m_mutex);
            m_engines.swap(engines);
        }
        qDeleteAll(engines);
    }
}

bool QClassPathEngine::copy(const QString& newName) {
    if(QAbstractFileEngine* afe = getFirstEngine())
        return afe->copy(newName);
    return false;
}

bool QClassPathEngine::setPermissions(uint perms) {
    if(QAbstractFileEngine* afe = getFirstEngine()){
        if(afe->setPermissions(perms)){
            setError(QFileDevice::NoError, {});
            return true;
        }else if(afe->error()!=QFileDevice::NoError)
            setError(afe->error(), afe->errorString());
    }
    return false;
}

bool QClassPathEngine::caseSensitive() const {
    bool result = false;
    if(QAbstractFileEngine* afe = getFirstEngine()){
        result |= afe->caseSensitive();
    }
    return result;
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
    for (QAbstractFileEngine* engine : qAsConst(engines)) {
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
    for (QAbstractFileEngine* engine : qAsConst(engines)) {
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

    for (QAbstractFileEngine* engine : qAsConst(engines))
        flags |= engine->fileFlags(type);

    if (fileName(PathName)==QStringLiteral(u"/"))
        flags |= QAbstractFileEngine::RootFlag;

    flags.setFlag(LocalDiskFlag, false);

    return flags;
}

QString QClassPathEngine::fileName(FileName file) const {
    QString result;
    switch(file){
    case DefaultName:
        result = m_prefix + m_fileName;
        break;
    case CanonicalName:
        result = fileName(CanonicalPathName) + QChar(u'/') + fileName(BaseName);
        break;
    case AbsoluteName:
#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
    case LinkName:
#endif
        result = m_prefix + m_baseName;
        break;
    case BaseName:{
        auto pos = m_baseName.lastIndexOf(u'/');
        result = pos > 0 ? m_baseName.mid(pos + 1) : m_baseName;
    }
    break;
    case CanonicalPathName:
    case AbsolutePathName:
    case PathName:
    default:{
        auto pos = m_baseName.lastIndexOf(u'/');
        result = m_prefix + (pos > 0 ? m_baseName.mid(0, pos) : "");
    }
    break;
    }
    return result;
}

QDateTime QClassPathEngine::fileTime(FileTime time) const {
    if(QAbstractFileEngine* afe = getFirstEngine())
        return afe->fileTime(time);
    return QDateTime();
}

bool QClassPathEngine::open(QIODevice::OpenMode openMode
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
                            ,std::optional<QFile::Permissions> permissions
#endif
                            ) {
    if(!openMode.testFlag(QIODevice::WriteOnly)){
        if(QAbstractFileEngine* afe = getFirstEngine()){
            bool result = afe->open(openMode
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
                                    ,permissions
#endif
                                    );
            setError(afe->error(), afe->errorString());
            return result;
        }
    }else{
        setError(QFileDevice::OpenError, "Unable to open resource path for writing");
    }
    return false;
}

qint64 QClassPathEngine::pos() const {
    if(QAbstractFileEngine* afe = getFirstEngine())
        return afe->pos();
    return -1;
}

qint64 QClassPathEngine::read(char *data, qint64 maxlen) {
    qint64 b = -1;
    if(QAbstractFileEngine* afe = getFirstEngine()){
        b = afe->read(data, maxlen);
        if(b<=0 && afe->error()!=QFileDevice::NoError)
            setError(afe->error(), afe->errorString());
    }
    return b;
}

qint64 QClassPathEngine::readLine(char *data, qint64 maxlen) {
    qint64 b = -1;
    if(QAbstractFileEngine* afe = getFirstEngine()){
        b = afe->readLine(data, maxlen);
        if(b<=0 && afe->error()!=QFileDevice::NoError)
            setError(afe->error(), afe->errorString());
    }
    return b;
}

bool QClassPathEngine::seek(qint64 offset) {
    bool b = false;
    if(QAbstractFileEngine* afe = getFirstEngine()){
        b = afe->seek(offset);
        if(!b && afe->error()!=QFileDevice::NoError)
            setError(afe->error(), afe->errorString());
    }
    return b;
}

QString QClassPathEngine::owner(FileOwner owner) const {
    QList<QAbstractFileEngine*> engines;
    {
        QMutexLocker locker(&m_mutex);
        engines = m_engines;
    }
    for (QAbstractFileEngine* engine : qAsConst(engines)){
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
    for (QAbstractFileEngine* engine : qAsConst(engines)){
        uint result = engine->ownerId(owner);
        if (result != uint(-2))
            return result;
    }
    return uint(-2);
}

bool QClassPathEngine::isSequential() const {
    if(QAbstractFileEngine* afe = getFirstEngine()){
        if (afe->isSequential())
            return true;
    }
    return false;
}

qint64 QClassPathEngine::size() const{
    if(QAbstractFileEngine* afe = getFirstEngine())
        return afe->size();
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
    for (QAbstractFileEngine* engine : qAsConst(engines)){
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
    for (QAbstractFileEngine* engine : qAsConst(engines)){
        entries << engine->entryList(filters, nameFilters);
    }
    entries.removeDuplicates();
    return QClassPathEngine::IteratorPtr(new QClassPathEngineIterator(entries, filters, nameFilters));
}
#endif

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

QClassPathFileEngineHandler::QAbstractFileEnginePointer QClassPathFileEngineHandler::create(const QString &givenFileName) const
{
    QClassPathEngine* result = nullptr;
    QLatin1String prefix;
    bool mayBeResource = false;
    if (QClassPathEngine::checkAndSetPrefix(givenFileName, QClassPathEngine::fileNamePrefix1, prefix)
        || QClassPathEngine::checkAndSetPrefix(givenFileName, QClassPathEngine::fileNamePrefix2, prefix)
        || QClassPathEngine::checkAndSetPrefix(givenFileName, QClassPathEngine::fileNamePrefix3, prefix)
        || (mayBeResource = QClassPathEngine::checkAndSetPrefix(givenFileName, QClassPathEngine::fileNamePrefix4, prefix))){
        QStringList resourceEntries;

        class AccessResource : public QResource{
            using QResource::QResource;
        public:
            static bool needsClassPathEngine(const QString &fileName, QStringList& resourceEntries){
                AccessResource resource(fileName);
                if(resource.isValid()){
                    resourceEntries << resource.children();
                    return !resourceEntries.isEmpty();
                }
                return true;
            }
        };

        if(!mayBeResource || AccessResource::needsClassPathEngine(givenFileName, resourceEntries)){
            QString fileName;
            QString baseName;
            QString selectedSource;
            QList<QAbstractFileEngine*> engines;
            QClassPathEngine::initialize(prefix, fileName, baseName, selectedSource, engines, givenFileName);
            if(!engines.isEmpty()){
                result = new QClassPathEngine(prefix,
                                              std::move(fileName),
                                              std::move(baseName),
                                              std::move(selectedSource),
                                              std::move(engines),
                                              std::move(resourceEntries));
            };
        }
    }
    return QAbstractFileEnginePointer(result);
}
