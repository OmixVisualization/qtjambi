/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/QAbstractItemModel>
#include "modelapi.h"
#include <QtCore/private/qobject_p.h>

struct CachedValue{
    int value = 0;
    bool isCached = false;
};

struct ModelDataPrivate{
    CachedValue rowCount;
    CachedValue columnCount;
    QHash<QModelIndex,CachedValue> rowCounts;
    QHash<QModelIndex,CachedValue> columnCounts;
    QMetaObject::Connection rowInsertedConnection;
    QMetaObject::Connection columnInsertedConnection;
    QMetaObject::Connection rowRemovedConnection;
    QMetaObject::Connection columnRemovedConnection;
};

ModelData::ModelData() : d(new ModelDataPrivate){
}

ModelData::~ModelData(){
    if(d->rowInsertedConnection)
        QObject::disconnect(d->rowInsertedConnection);
    if(d->columnInsertedConnection)
        QObject::disconnect(d->columnInsertedConnection);
    if(d->rowRemovedConnection)
        QObject::disconnect(d->rowRemovedConnection);
    if(d->columnRemovedConnection)
        QObject::disconnect(d->columnRemovedConnection);
    delete d;
}

bool ModelData::get_rowCount(const QAbstractItemModel* model, int& value, const QModelIndex& parent) const{
    if(d->rowInsertedConnection && d->rowRemovedConnection){
        if(!parent.isValid()){
            if(d->rowCount.isCached)
                value = d->rowCount.value;
            return d->rowCount.isCached;
        }else{
            CachedValue rowCount = d->rowCounts.value(parent);
            if(rowCount.isCached)
                value = rowCount.value;
            return rowCount.isCached;
        }
    }else{
        if(d->rowInsertedConnection)
            QObject::disconnect(d->rowInsertedConnection);
        if(d->rowRemovedConnection)
            QObject::disconnect(d->rowRemovedConnection);
        d->rowInsertedConnection = QObject::connect(model, &QAbstractItemModel::rowsInserted, model, [this](){
                d->rowCount.isCached = false;
                d->rowCounts.clear();
            }, Qt::DirectConnection);
        d->rowRemovedConnection = QObject::connect(model, &QAbstractItemModel::rowsRemoved, model, [this](){
                d->rowCount.isCached = false;
                d->rowCounts.clear();
            }, Qt::DirectConnection);
    }
    return false;
}

void ModelData::set_rowCount(int value, const QModelIndex& parent){
    if(!parent.isValid()){
        d->rowCount = CachedValue{value, true};
    }else{
        d->rowCounts[parent] = CachedValue{value, true};
    }
}

bool ModelData::get_columnCount(const QAbstractItemModel* model, int& value, const QModelIndex& parent) const{
    if(d->columnInsertedConnection && d->columnRemovedConnection){
        if(!parent.isValid()){
            if(d->columnCount.isCached)
                value = d->columnCount.value;
            return d->columnCount.isCached;
        }else{
            CachedValue columnCount = d->columnCounts.value(parent);
            if(columnCount.isCached)
                value = columnCount.value;
            return columnCount.isCached;
        }
    }else{
        if(d->columnInsertedConnection)
            QObject::disconnect(d->columnInsertedConnection);
        if(d->columnRemovedConnection)
            QObject::disconnect(d->columnRemovedConnection);
        d->columnInsertedConnection = QObject::connect(model, &QAbstractItemModel::columnsInserted, model, [this](){
                d->columnCount.isCached = false;
                d->columnCounts.clear();
            }, Qt::DirectConnection);
        d->columnRemovedConnection = QObject::connect(model, &QAbstractItemModel::columnsRemoved, model, [this](){
                d->columnCount.isCached = false;
                d->columnCounts.clear();
            }, Qt::DirectConnection);
    }
    return false;
}

void ModelData::set_columnCount(int value, const QModelIndex& parent){
    if(!parent.isValid()){
        d->columnCount = CachedValue{value, true};
    }else{
        d->columnCounts[parent] = CachedValue{value, true};
    }
}

struct Dummy : public JniEnvironment{
public:
    quint8 data;
};

void JniEnvironmentExceptionModelHandler::handleException(const JavaException& exn, const QAbstractItemModel* model, const char* methodName){
    bool blockException = false;
    if(model && reinterpret_cast<Dummy*>(this)->data==0){
        const QObjectPrivate* p = QObjectPrivate::get(model);
        blockException = !p->isDeletingChildren && p->declarativeData;
    }
    //static ResettableBoolFlag noExceptionForwardingByMetaObject("io.qt.no-exception-forwarding-from-");
    if(blockException/* || noExceptionForwardingByMetaObject*/){
        QtJambiExceptionBlocker __blocker;
        {
            QtJambiExceptionHandler __handler;
            __handler.handle(environment(), exn, nullptr);
        }
        __blocker.release(environment());
        return;
    }
    JniEnvironmentExceptionHandler::handleException(exn, methodName);
}

void JniEnvironmentScopeExceptionModelHandler::handleException(const JavaException& exn, const QAbstractItemModel* model, const char* methodName){
    bool blockException = false;
    if(model && reinterpret_cast<Dummy*>(this)->data==0){
        const QObjectPrivate* p = QObjectPrivate::get(model);
        blockException = !p->isDeletingChildren && p->declarativeData;
    }
    //static ResettableBoolFlag noExceptionForwardingByMetaObject("io.qt.no-exception-forwarding-from-");
    if(blockException/* || noExceptionForwardingByMetaObject*/){
        QtJambiExceptionBlocker __blocker;
        {
            QtJambiExceptionHandler __handler;
            __handler.handle(environment(), exn, nullptr);
        }
        __blocker.release(environment());
        return;
    }
    JniEnvironmentScopeExceptionHandler::handleException(exn, methodName);
}

void JniEnvironmentScopeExceptionModelInhibitor::handleException(const JavaException& exn, const QAbstractItemModel*, const char* methodName){
    JniEnvironmentScopeExceptionInhibitor::handleException(exn, methodName);
}

void JniEnvironmentScopeExceptionModelHandlerAndBlocker::handleException(const JavaException& exn, const QAbstractItemModel*, const char* methodName){
    JniEnvironmentScopeExceptionHandlerAndBlocker::handleException(exn, methodName);
}

void JniEnvironmentScopeExceptionModelInhibitorAndBlocker::handleException(const JavaException& exn, const QAbstractItemModel*, const char* methodName){
    JniEnvironmentScopeExceptionInhibitorAndBlocker::handleException(exn, methodName);
}

void JniEnvironmentExceptionModelInhibitorAndBlocker::handleException(const JavaException& exn, const QAbstractItemModel*, const char* methodName){
    JniEnvironmentExceptionInhibitorAndBlocker::handleException(exn, methodName);
}
