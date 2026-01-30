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

#include <QtCore/private/qobject_p.h>

#include "pch_p.h"

namespace QtJambiPrivate{

struct ObjectDataContainerPrivate : public QSharedData {
    ObjectDataContainerPrivate();
    ~ObjectDataContainerPrivate();
    QVector<size_t> indexes;
    QVector<QtJambiObjectData*> data;
};

class ObjectDataContainer{
public:
    ObjectDataContainer();
    ObjectDataContainer(const std::type_info& id, QtJambiObjectData* data);
    ObjectDataContainer(const ObjectDataContainer& other);
    ObjectDataContainer(ObjectDataContainer&& other);
    ~ObjectDataContainer();
    ObjectDataContainer& operator=(const ObjectDataContainer& other);
    ObjectDataContainer& operator=(ObjectDataContainer&& other);
    bool operator==(const ObjectDataContainer& other);
    void swap(ObjectDataContainer& other);
    QtJambiObjectData* setUserData(const std::type_info& id, QtJambiObjectData* data);
    QtJambiObjectData* userData(const std::type_info& id) const;
    bool isEmpty() const;
private:
    QExplicitlySharedDataPointer<ObjectDataContainerPrivate> p;
};

}

Q_DECLARE_METATYPE(QtJambiPrivate::ObjectDataContainer)

void swap(QtJambiPrivate::ObjectDataContainer& a, QtJambiPrivate::ObjectDataContainer& b){
    a.swap(b);
}

namespace QtJambiPrivate{

ObjectDataContainerPrivate::ObjectDataContainerPrivate() : indexes(), data() {
    indexes.fill(0, 3);
    data.fill(nullptr, 3);
}

ObjectDataContainerPrivate::~ObjectDataContainerPrivate(){
    QVector<QtJambiObjectData*> _data;
    if(QtJambiStorageOptional storage{}){
        QWriteLocker locker(storage->lock());
        data.swap(_data);
        indexes.clear();
    }else{
        data.swap(_data);
        indexes.clear();
    }
    qDeleteAll(_data);
}

ObjectDataContainer::ObjectDataContainer()
    : p(new ObjectDataContainerPrivate)
{}

ObjectDataContainer::ObjectDataContainer(const ObjectDataContainer& other)
    : p(other.p)
{}

ObjectDataContainer::ObjectDataContainer(ObjectDataContainer&& other)
    : p(std::move(other.p))
{}

ObjectDataContainer::ObjectDataContainer(const std::type_info& id, QtJambiObjectData* data)
    : ObjectDataContainer()
{
    (void)setUserData(id, data);
}

ObjectDataContainer::~ObjectDataContainer(){}

ObjectDataContainer& ObjectDataContainer::operator=(const ObjectDataContainer& other){
    p = other.p;
    return *this;
}

ObjectDataContainer& ObjectDataContainer::operator=(ObjectDataContainer&& other){
    p = std::move(other.p);
    return *this;
}

bool ObjectDataContainer::operator==(const ObjectDataContainer& other){
    return p==other.p;
}

void ObjectDataContainer::swap(ObjectDataContainer& other){
    p.swap(other.p);
}

bool ObjectDataContainer::isEmpty()const{
    if(p){
        for(QtJambiObjectData* d : std::as_const(p->data)){
            if(d)
                return false;
        }
    }
    return true;
}

QtJambiObjectData* ObjectDataContainer::setUserData(const std::type_info& id, QtJambiObjectData* data){
    auto idx = p->indexes.indexOf(unique_id(id));
    if(idx<0){
        if(!data)
            return nullptr;
        idx = p->indexes.size();
        if(p->indexes.size()<=idx)
            p->indexes.resize(idx+1, 0);
        p->indexes[idx] = unique_id(id);
    }
    if(p->data.size()<=idx){
        if(!data)
            return nullptr;
        p->data.resize(idx+1, nullptr);
    }
    QtJambiObjectData* current = p->data[idx];
    p->data[idx] = data;
    return current;
}

QtJambiObjectData* ObjectDataContainer::userData(const std::type_info& id) const{
    auto idx = p->indexes.indexOf(unique_id(id));
    return idx>=0 && idx<p->data.size() ? p->data.value(idx) : nullptr;
}

}//namespace QtJambiPrivate

bool useHiddenObjectData(JNIEnv *env = nullptr){
    static bool result = [](JNIEnv *env)->bool{
        Q_ASSERT(env);
        try{
            if(Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.no-hidden-objectdata"))){
                return false;
            }
        }catch(const JavaException& exn){exn.report(env);}
        return true;
    }(env);
    return result;
}

QtJambiObjectData::QtJambiObjectData(){}
QtJambiObjectData::~QtJambiObjectData(){}

struct QtJambiObjectDataReadLockPrivate{
    QtJambiObjectDataReadLockPrivate() : locker(getQtJambiStorage()->linkLock()) {
    }
    void unlock(){
        locker.unlock();
    }
    void relock(){
        locker.relock();
    }
private:
    QReadLocker locker;
};

QtJambiObjectDataReadLock::QtJambiObjectDataReadLock(QtJambiObjectDataReadLockPrivate&_p) : p(&_p){
}

QtJambiObjectDataReadLock::~QtJambiObjectDataReadLock(){
    delete p;
}

void QtJambiObjectDataReadLock::unlock(){
    p->unlock();
}

void QtJambiObjectDataReadLock::relock(){
    p->relock();
}

struct QtJambiObjectDataWriteLockPrivate{
    QtJambiObjectDataWriteLockPrivate() : locker(getQtJambiStorage()->linkLock()) {
    }
    void unlock(){
        locker.unlock();
    }
    void relock(){
        locker.relock();
    }
private:
    QWriteLocker locker;
};

QtJambiObjectDataWriteLock::QtJambiObjectDataWriteLock(QtJambiObjectDataWriteLockPrivate&_p) : p(&_p){
}

QtJambiObjectDataWriteLock::~QtJambiObjectDataWriteLock(){
    delete p;
}

void QtJambiObjectDataWriteLock::unlock(){
    p->unlock();
}

void QtJambiObjectDataWriteLock::relock(){
    p->relock();
}

QtJambiObjectDataWriteLock QtJambiObjectData::writeLock()
{
    return QtJambiObjectDataWriteLock(*new QtJambiObjectDataWriteLockPrivate());
}

QtJambiObjectDataReadLock QtJambiObjectData::readLock()
{
    return QtJambiObjectDataReadLock(*new QtJambiObjectDataReadLockPrivate());
}

QtJambiObjectData* QtJambiObjectData::userData(const QObject* object, const std::type_info& id)
{
    using namespace QtJambiPrivate;
    const QObjectPrivate* p = object ? QObjectPrivate::get(object) : nullptr;
    if(p && p->extraData && (!p->wasDeleted || typeid_not_equals(id, typeid(ValueOwnerObjectData)))){
        if(useHiddenObjectData()){
            const auto i = p->extraData->propertyNames.size();
            if(i>=0 && i<p->extraData->propertyValues.size()){
                const QVariant& variant = p->extraData->propertyValues.at(i);
                if(variant.metaType()==QMetaType::fromType<ObjectDataContainer>()){
                    if(const ObjectDataContainer* container = reinterpret_cast<const ObjectDataContainer*>(variant.data()))
                        return container->userData(id);
                }
            }
        }else{
            char name[sizeof(void*)+1];
            memcpy(name, &p->extraData, sizeof(void*));
            name[sizeof(void*)] = '\0';
            const auto i = p->extraData->propertyNames.indexOf(name);
            if(i>=0 && i<p->extraData->propertyValues.size()){
                const QVariant& variant = p->extraData->propertyValues.at(i);
                if(variant.metaType()==QMetaType::fromType<ObjectDataContainer>()){
                    if(const ObjectDataContainer* container = reinterpret_cast<const ObjectDataContainer*>(variant.data()))
                        return container->userData(id);
                }
            }
        }
    }
    return nullptr;
}

QtJambiObjectData* QtJambiObjectData::setUserData(QObject* object, const std::type_info& id, QtJambiObjectData* data)
{
    using namespace QtJambiPrivate;
    QtJambiObjectData* old = nullptr;
    QObjectPrivate* p = object ? QObjectPrivate::get(object) : nullptr;
    if(p && (typeid_not_equals(id, typeid(ValueOwnerObjectData)) || !p->wasDeleted)){
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
        if (!p->extraData)
            p->extraData = new QObjectPrivate::ExtraData;
#elif QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
        if (!p->extraData)
            p->extraData = new QObjectPrivate::ExtraData(p);
#else
        p->ensureExtraData();
#endif
        if(useHiddenObjectData()){
            const auto i = p->extraData->propertyNames.size();
            if(i<p->extraData->propertyValues.size()
                    && p->extraData->propertyValues[i].metaType()==QMetaType::fromType<ObjectDataContainer>()){
                if(ObjectDataContainer* container = reinterpret_cast<ObjectDataContainer*>(p->extraData->propertyValues[i].data())){
                    old = container->setUserData(id, data);
                    if(!data && container->isEmpty()){
                        p->extraData->propertyValues.takeAt(i);
                    }
                }
            }else if(data){
                if(i<p->extraData->propertyValues.size()){
                    p->extraData->propertyValues.replace(i, QVariant::fromValue<ObjectDataContainer>({id, data}));
                }else{
                    p->extraData->propertyValues.append(QVariant::fromValue<ObjectDataContainer>({id, data}));
                }
            }
        }else{
            char name[sizeof(void*)+1];
            memcpy(name, &p->extraData, sizeof(void*));
            name[sizeof(void*)] = '\0';
            const auto i = p->extraData->propertyNames.indexOf(name);
            if(i>=0 && p->extraData->propertyValues[i].metaType()==QMetaType::fromType<ObjectDataContainer>()){
                if(ObjectDataContainer* container = reinterpret_cast<ObjectDataContainer*>(p->extraData->propertyValues[i].data())){
                    old = container->setUserData(id, data);
                    if(!data && container->isEmpty()){
                        p->extraData->propertyNames.takeAt(i);
                        p->extraData->propertyValues.takeAt(i);
                    }
                }
            }else if(data){
                if(i<0){
                    p->extraData->propertyNames.append(name);
                    p->extraData->propertyValues.append(QVariant::fromValue<ObjectDataContainer>({id, data}));
                }else{
                    p->extraData->propertyValues.replace(i, QVariant::fromValue<ObjectDataContainer>({id, data}));
                }
            }
        }
    }
    return old;
}

bool QtJambiObjectData::isRejectedUserProperty(const QObject* object, const char * propertyName) {
    using namespace QtJambiPrivate;
    const QObjectPrivate* p = object ? QObjectPrivate::get(object) : nullptr;
    if(propertyName && p && p->extraData && !useHiddenObjectData()){
        char name[sizeof(void*)+1];
        memcpy(name, &p->extraData, sizeof(void*));
        name[sizeof(void*)] = '\0';
        return strcmp(name, propertyName)==0;
    }
    return false;
}

void forceRemoveObjectData(QObject* object){
    using namespace QtJambiPrivate;
    QVariant variant;
    {
        auto locker = QtJambiObjectData::writeLock();
        QObjectPrivate* p = object ? QObjectPrivate::get(object) : nullptr;
        if(p && p->extraData && !p->wasDeleted){
            if(useHiddenObjectData()){
                const auto i = p->extraData->propertyNames.size();
                if(i<p->extraData->propertyValues.size()
                    && p->extraData->propertyValues[i].metaType()==QMetaType::fromType<ObjectDataContainer>()){
                    variant = p->extraData->propertyValues.takeAt(i);
                }
            }else{
                char name[sizeof(void*)+1];
                memcpy(name, &p->extraData, sizeof(void*));
                name[sizeof(void*)] = '\0';
                const auto i = p->extraData->propertyNames.indexOf(name);
                if(i>=0 && p->extraData->propertyValues[i].metaType()==QMetaType::fromType<ObjectDataContainer>()){
                    p->extraData->propertyNames.takeAt(i);
                    variant = p->extraData->propertyValues.takeAt(i);
                }
            }
        }
    }
}

void onDynamicPropertyChange(QObject *object, QDynamicPropertyChangeEvent* event){
    using namespace QtJambiPrivate;
    if(useHiddenObjectData() && event){
        QObjectPrivate* p = object ? QObjectPrivate::get(object) : nullptr;
        if(p && !p->wasDeleted && p->extraData){
            // check if property has been appended:
            if(!p->extraData->propertyNames.isEmpty() && p->extraData->propertyNames.last()==event->propertyName()){
                if(p->extraData->propertyNames.size()==p->extraData->propertyValues.size()-1
                        && p->extraData->propertyValues[p->extraData->propertyValues.size()-2].metaType()==QMetaType::fromType<ObjectDataContainer>()){
                    p->extraData->propertyValues[p->extraData->propertyValues.size()-2].swap(p->extraData->propertyValues[p->extraData->propertyValues.size()-1]);
                }
            }
        }
    }
}
