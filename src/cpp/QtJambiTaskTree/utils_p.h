/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
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

#ifndef UTILS_P_H
#define UTILS_P_H

#include <QtTaskTree/QTaskTree>
#include <QtCore/QVariant>
#include <QtCore/QVariant>
#include <QtCore/QBitArray>
#include <QtCore/QUuid>
#include <QtCore/QJsonObject>
#include <QtCore/QCborMap>
#include <QtCore/QCborArray>
#include <QtCore/QPromise>
#include <QtCore/QPointer>
#include <QtJambi/Global>

#include <QtJambi/QtJambiAPI>
#include <QtJambi/ContainerAPI>
#include <QtJambi/FutureAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/JObjectWrapper>
#include <QtJambi/Cast>

namespace Java{
namespace QtTaskTree{
QTJAMBI_REPOSITORY_DECLARE_CLASS(Timeout,
                                 QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                 QTJAMBI_REPOSITORY_DECLARE_LONG_WRITABLE_FIELD(__qt_directLink))
QTJAMBI_REPOSITORY_DECLARE_CLASS(Storage$ActiveStorage,
                                 QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
}
}

std::initializer_list<QtTaskTree::GroupItem> convertGroupItems(JNIEnv* env, jobjectArray array);
void createGroupItem(void* placement);
QtTaskTree::ExecutableItem executableItem();
void createExecutableItem(void* placement);
QObject* taskOfQProcessTask(void *taskAdapter);
QObject* taskOfQTaskTreeTask(void *taskAdapter);
QObject* taskOfQBarrierTask(void *taskAdapter);

inline void containerDisposer(AbstractContainerAccess* _access){
    if(_access)
        _access->dispose();
}

struct Container{
    void* list;
    AbstractListAccess* access;
    Container(const void* _list, AbstractListAccess* _access)
        : list(_access->createContainer(_list)), access(_access->clone())
    {}
    ~Container(){
        access->deleteContainer(list);
        access->dispose();
    }
    size_t size() const {
        return access->size(list);
    }
    const QMetaType& elementMetaType() const {
        return access->elementMetaType();
    }
    void* at(qsizetype i){
        return access->at(list, i);
    }
    jobject at(JNIEnv* env, jint i){
        return access->at(env, list, i);
    }
};

jobject convertTaskInterface(JNIEnv* env, QtTaskTree::QTaskInterface *iface);

namespace QtTaskTree{
template <>
class ListIterator<QVariant> final : public Iterator
{
public:
    explicit ListIterator(const QPair<const void*,AbstractListAccess*>& info)
        : ListIterator(std::make_shared<Container>(info.first,info.second)) { }
    QVariant operator*() const {
        return iteration()<0 ? QVariant() : QVariant(m_container->elementMetaType(), valuePtr());
    }
    jobject value(JNIEnv* env) const {
        qsizetype i = iteration();
        return i>=0 && i < qsizetype(m_container->size()) ? m_container->at(env, jint(i)) : nullptr;
    }
private:
    ListIterator(const std::shared_ptr<Container>& container)
      : Iterator(container->size(),[container](qsizetype i) -> void* { return container->at(i); }),
        m_container(container) { }
    std::shared_ptr<Container> m_container;
};

inline jobject getValue(JNIEnv* env, const QtTaskTree::ListIterator<QVariant> *iterator){
    return iterator->value(env);
}

template <>
class QDefaultTaskAdapter<JObjectWrapper>
{
    JObjectWrapper m_adapter;
public:
    QDefaultTaskAdapter(JNIEnv* env, jobject adapter) : m_adapter(env, adapter){}
    void operator()(JObjectWrapper *task, QTaskInterface *iface) const {
        if(JniEnvironment env{128}){
            Java::Runtime::BiConsumer::accept(env, task->object(env), convertTaskInterface(env, iface));
        }
    }
};

template <>
class QCustomTask<JObjectWrapper> final : public ExecutableItem
{
public:
    using Task = JObjectWrapper;
    using Adapter = QDefaultTaskAdapter<JObjectWrapper>;
    using TaskSetupHandler = std::function<SetupResult(Task &)>;
    using TaskDoneHandler = std::function<DoneResult(const Task &, DoneWith)>;

    template <typename SetupHandler, typename DoneHandler>
    explicit QCustomTask(JNIEnv* env, jobject taskFactory, jobject adapterFactory,
                         SetupHandler &&setup,
                         DoneHandler &&done,
                         CallDone callDone)
        : ExecutableItem(TaskHandler{taskAdapterConstructor(env, taskFactory, adapterFactory),
#if QT_VERSION < QT_VERSION_CHECK(6,12,0)
                                     &taskAdapterDestructor,
#endif
                                     &taskAdapterStarter,
                                     wrapSetup(std::forward<SetupHandler>(setup)),
                                     wrapDone(std::forward<DoneHandler>(done)),
                                     callDone})
    {}

    struct TaskAdapter {
        TaskAdapter(JNIEnv* env, jobject task, jobject adapter)
            : task(new Task(env, task)), adapter(env, adapter)
        {
            if(Java::QtCore::QObject::isInstanceOf(env, task)){
                object = QtJambiAPI::convertJavaObjectToQObject(env, task);
            }
        }
        std::unique_ptr<Task> task;
        QPointer<QObject> object;
        Adapter adapter;
    };
private:
    friend class When;


#if QT_VERSION >= QT_VERSION_CHECK(6,12,0)
    static TaskAdapterCreator taskAdapterConstructor(JNIEnv* env, jobject taskFactory, jobject adapterFactory) {
        return [taskFactory = JObjectWrapper(env, taskFactory), adapterFactory = JObjectWrapper(env, adapterFactory)]() -> std::shared_ptr<void> {
            if(JniEnvironment env{128}){
                return std::make_shared<TaskAdapter>(env, Java::Runtime::Supplier::get(env, taskFactory.object(env)), Java::Runtime::Supplier::get(env, adapterFactory.object(env)));
            }else return std::shared_ptr<void>(nullptr);
        };
    }
#else
    static TaskAdapterConstructor taskAdapterConstructor(JNIEnv* env, jobject taskFactory, jobject adapterFactory) {
        return [taskFactory = JObjectWrapper(env, taskFactory), adapterFactory = JObjectWrapper(env, adapterFactory)]() -> TaskAdapter* {
            if(JniEnvironment env{128}){
                return new TaskAdapter(env, Java::Runtime::Supplier::get(env, taskFactory.object(env)), Java::Runtime::Supplier::get(env, adapterFactory.object(env)));
            }else return nullptr;
        };
    }
    static void taskAdapterDestructor(TaskAdapterPtr voidAdapter) {
        delete static_cast<TaskAdapter *>(voidAdapter);
    }
#endif
    static void taskAdapterStarter(TaskAdapterPtr voidAdapter, QTaskInterface *iface) {
        TaskAdapter *taskAdapter = static_cast<TaskAdapter *>(voidAdapter);
        std::invoke(taskAdapter->adapter, taskAdapter->task.get(), iface);
    }

    template <typename Handler>
    static TaskAdapterSetupHandler wrapSetup(Handler &&handler) {
        if constexpr (std::is_same_v<std::decay_t<Handler>, TaskSetupHandler>) {
            if (!handler)
                return {}; // User passed {} for the setup handler.
        }
        return [handler = std::forward<Handler>(handler)](TaskAdapterPtr voidAdapter) {
            Task *task = static_cast<TaskAdapter *>(voidAdapter)->task.get();
            return std::invoke(handler, *task);
        };
    }

    template <typename Handler>
    static TaskAdapterDoneHandler wrapDone(Handler &&handler) {
        if constexpr (std::is_same_v<std::decay_t<Handler>, TaskDoneHandler>) {
            if (!handler)
                return {}; // User passed {} for the done handler.
        }
        return [handler = std::forward<Handler>(handler)](TaskAdapterPtr voidAdapter,
                                                          DoneWith result) {
            Task *task = static_cast<TaskAdapter *>(voidAdapter)->task.get();
            return std::invoke(handler, *task, result);
        };
    }
};

template <>
class Storage<QVariant> final : public StorageBase
{
public:
    Storage(JNIEnv* env, jclass structType, jobject supplier);
    QVariant *activeStorage() const;
    const JObjectWrapper& structType() const;
private:
#if QT_VERSION >= QT_VERSION_CHECK(6,12,0)
#else
    static StorageConstructor ctor(JNIEnv* env, jobject supplier);
    static StorageDestructor dtor();
#endif
    JObjectWrapper m_structType;
};
} // namespace QtTaskTree

template <>
struct std::hash<QVariant> {
    inline size_t _Do_hash(const QVariant& value) const noexcept {
        switch(value.userType()){
        case QMetaType::Type::Char: return qHash(value.value<char>());
        case QMetaType::Type::UChar: return qHash(value.value<uchar>());
        case QMetaType::Type::SChar: return qHash(value.value<signed char>());
        case QMetaType::Type::QChar: return qHash(value.value<QChar>());
        case QMetaType::Type::Short: return qHash(value.value<short>());
        case QMetaType::Type::UShort: return qHash(value.value<ushort>());
        case QMetaType::Type::Int: return qHash(value.value<int>());
        case QMetaType::Type::UInt: return qHash(value.value<uint>());
        case QMetaType::Type::Long: return qHash(value.value<long>());
        case QMetaType::Type::ULong: return qHash(value.value<ulong>());
        case QMetaType::Type::LongLong: return qHash(value.value<long long>());
        case QMetaType::Type::ULongLong: return qHash(value.value<unsigned long long>());
        case QMetaType::Type::Float: return qHash(value.value<float>());
        case QMetaType::Type::Double: return qHash(value.value<double>());
        case QMetaType::Type::Bool: return qHash(value.value<bool>());
        case QMetaType::Type::QStringList: return qHash(value.value<QStringList>());
        case QMetaType::Type::QByteArrayList: return qHash(value.value<QByteArrayList>());
        case QMetaType::Type::QString: return qHash(value.value<QString>());
        case QMetaType::Type::QByteArray: return qHash(value.value<QByteArray>());
        case QMetaType::Type::QBitArray: return qHash(value.value<QBitArray>());
        case QMetaType::Type::QUuid: return qHash(value.value<QUuid>());
        case QMetaType::Type::QJsonValue: return qHash(value.value<QJsonValue>());
        case QMetaType::Type::QJsonObject: return qHash(value.value<QJsonObject>());
        case QMetaType::Type::QCborValue: return qHash(value.value<QCborValue>());
        case QMetaType::Type::QCborArray: return qHash(value.value<QCborArray>());
        case QMetaType::Type::QCborMap: return qHash(value.value<QCborMap>());
        case QMetaType::Type::Float16: return qHash(value.value<qfloat16>());
        default: break;
        }
        if(value.metaType().flags() & QMetaType::IsPointer
            || value.metaType().flags() & QMetaType::PointerToQObject){
            return qHash(*reinterpret_cast<void*const*>(value.data()));
        }
        if(JniEnvironment env{128}){
            jobject obj = qtjambi_cast<jobject>(env, value);
            return Java::Runtime::Object::hashCode(env, obj);
        }
        return 0;
    }
    inline size_t operator()(const QVariant& value) const noexcept {
        return _Do_hash(value);
    }
};

namespace QtTaskTree{
class GroupItemPrivate{
public:
    using TaskHandler = GroupItem::TaskHandler;
    template <typename Task, typename Adapter, typename Deleter>
    static QtTaskTree::GroupItemPrivate::TaskHandler taskHandler(const QtTaskTree::QCustomTask<Task, Adapter, Deleter> &task){
        return task.taskHandler();
    }
};
}

QtTaskTree::ExecutableItem executableItem(const QtTaskTree::GroupItemPrivate::TaskHandler& h);

#endif // UTILS_P_H
