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

#include <QtTaskTree/qtasktree.h>
#include <QtTaskTree/qprocesstask.h>
#include "utils_p.h"
#include <QtCore/QObject>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/QtJambiArrayAPI>
#include <QtJambi/Cast>

class QProcess;

namespace QtTaskTree{
class QTaskTreePrivate{
public:
    static inline GroupItem* createGroupItemArray(size_t s){
        return new GroupItem[s];
    }
    static inline GroupItem* createGroupItem(){
        return new GroupItem();
    }
    static inline GroupItem* createGroupItem(void* placement){
        return new(placement) GroupItem();
    }
    static inline GroupItem groupItem(){
        return GroupItem();
    }
    static inline GroupItem groupHandler(const QtTaskTree::GroupItem::GroupSetupHandler& setupHandler, const QtTaskTree::GroupItem::GroupDoneHandler& doneHandler, QtTaskTree::CallDone callDoneFlags){
        return GroupItem::groupHandler({setupHandler,doneHandler,callDoneFlags});
    }
    static bool equals(const GroupItem& value1, const GroupItem& value2){
        return value1.d==value2.d;
    }
};

class QBarrier;
class QProcessTaskAdapter;
class QProcessTaskDeleter;
class QStartedBarrier;

using QStoredBarrier = Storage<QStartedBarrier>;

class When{
public:
    static inline ExecutableItem* createExecutableItemArray(size_t s){
        return new ExecutableItem[s];
    }
    static inline ExecutableItem* createExecutableItem(){
        return new ExecutableItem();
    }
    static inline ExecutableItem* createExecutableItem(void* placement){
        return new(placement) ExecutableItem();
    }
    static inline ExecutableItem executableItem(){
        return ExecutableItem();
    }
    static inline ExecutableItem executableItem(const QtTaskTree::ExecutableItem& item){
        return ExecutableItem(item);
    }
    static inline ExecutableItem executableItem(const QtTaskTree::ExecutableItem::TaskHandler& h){
        return ExecutableItem(h);
    }

    static inline Group withCancelImpl(ExecutableItem* ei, const std::function<void(QObject *, const std::function<void()> &)> &connectWrapper,
                                       const GroupItems &postCancelRecipe)
    {
        return ei->withCancelImpl(connectWrapper, postCancelRecipe);
    }

    static inline Group withAcceptImpl(ExecutableItem* ei, const std::function<void(QObject *, const std::function<void()> &)> &connectWrapper)
    {
        return ei->withAcceptImpl(connectWrapper);
    }

    template <typename Task>
    static inline QObject* task(void *taskAdapter){
        using TaskAdapter = typename Task::TaskAdapter;
        TaskAdapter *adapter = static_cast<TaskAdapter *>(taskAdapter);
        return adapter->task.get();
    }
};
}

QObject* taskOfQProcessTask(void *taskAdapter){
    return QtTaskTree::When::task<QtTaskTree::QProcessTask>(taskAdapter);
}

QObject* taskOfQTaskTreeTask(void *taskAdapter){
    return QtTaskTree::When::task<QtTaskTree::QTaskTreeTask>(taskAdapter);
}

QObject* taskOfQBarrierTask(void *taskAdapter){
    return QtTaskTree::When::task<QtTaskTree::QTaskTreeTask>(taskAdapter);
}

bool operator==(const QtTaskTree::GroupItem& value1, const QtTaskTree::GroupItem& value2){
    return QtTaskTree::QTaskTreePrivate::equals(value1, value2);
}

QtTaskTree::GroupItem groupHandler(const QtTaskTree::GroupItem::GroupSetupHandler& setupHandler, const QtTaskTree::GroupItem::GroupDoneHandler& doneHandler, QtTaskTree::CallDone callDoneFlags)
{
    return QtTaskTree::QTaskTreePrivate::groupHandler(setupHandler,doneHandler,callDoneFlags);
}

QtTaskTree::Group withCancel(QtTaskTree::ExecutableItem* ei, const std::function<void(QObject *, const std::function<void()> &)> &connectWrapper,
                                   const QtTaskTree::GroupItems &postCancelRecipe)
{
    return QtTaskTree::When::withCancelImpl(ei, connectWrapper, postCancelRecipe);
}

QtTaskTree::Group withAccept(QtTaskTree::ExecutableItem* ei, const std::function<void(QObject *, const std::function<void()> &)> &connectWrapper)
{
    return QtTaskTree::When::withAcceptImpl(ei, connectWrapper);
}

QtTaskTree::ExecutableItem executableItem(){
    return QtTaskTree::When::executableItem();
}

QtTaskTree::ExecutableItem executableItem(const QtTaskTree::ExecutableItem& item){
    return QtTaskTree::When::executableItem(item);
}

QtTaskTree::ExecutableItem executableItem(const QtTaskTree::GroupItemPrivate::TaskHandler& h){
    return QtTaskTree::When::executableItem(h);
}

void createExecutableItem(void* placement){
    QtTaskTree::When::createExecutableItem(placement);
}

void createGroupItem(void* placement){
    QtTaskTree::QTaskTreePrivate::createGroupItem(placement);
}

std::initializer_list<QtTaskTree::GroupItem> convertGroupItems(JNIEnv* env, jobjectArray array){
    jsize size = array ? env->GetArrayLength(array) : 0;
    if(size>0){
        QtTaskTree::GroupItem* groupItems = QtTaskTree::QTaskTreePrivate::createGroupItemArray(size);
        for(jsize i=0; i<size; ++i){
            jobject element = env->GetObjectArrayElement(array, i);
            const QtTaskTree::GroupItem * item = qtjambi_cast<const QtTaskTree::GroupItem *>(env, element);
            if(item)
                groupItems[i] = *item;
        }
        return QtJambiAPI::initializer_list<QtTaskTree::GroupItem>(groupItems, size);
    }else{
        return {};
    }
}