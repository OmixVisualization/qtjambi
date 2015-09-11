/****************************************************************************
**
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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



#include "qdeclarativelistproperty.h"
#include <QtDeclarative>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtdynamicmetaobject.h>
#include <qtjambi/qtjambitypemanager_p.h>


JNIEXPORT jclass JNICALL Java_org_qtjambi_qt_declarative_QDeclarativeListReference__1_1qt_1listElementType
  (JNIEnv * env, jobject, jlong this_pointer){
    QDeclarativeListReference *_this = (QDeclarativeListReference *) qtjambi_from_jlong(this_pointer);
    const QMetaObject * type = _this->listElementType();
    if(qtjambi_metaobject_is_dynamic(type)){
        const QtDynamicMetaObject* dynamicType = static_cast<const QtDynamicMetaObject*>(type);
        jclass cls = env->FindClass(dynamicType->javaClassName().toLatin1().constData());
        if (env->ExceptionCheck()) {
            env->Throw(env->ExceptionOccurred());
        }
        return cls;
    }else{
        QtJambiTypeManager manager(env, true, QtJambiTypeManager::DynamicMetaObjectMode);
        QString javaName = manager.getExternalTypeName(QLatin1String(type->className()), QtJambiTypeManager::ArgumentType);
        jclass cls = env->FindClass(javaName.replace(".", "/").toLatin1().constData());
        if (env->ExceptionCheck()) {
            env->Throw(env->ExceptionOccurred());
        }
        return cls;
    }
}

QtjambiDeclarativeListProperty::~QtjambiDeclarativeListProperty(){
    if(m_deleteFunction){
        m_deleteFunction(data);
    }
}

QtjambiDeclarativeListProperty::QtjambiDeclarativeListProperty() :
    QDeclarativeListProperty(),
    m_copyFunction(0),
    m_deleteFunction(0) {}
QtjambiDeclarativeListProperty::QtjambiDeclarativeListProperty(const QtjambiDeclarativeListProperty& property) :
    QDeclarativeListProperty(
            ((QDeclarativeListProperty*)&property)->object,
            0,
            ((QDeclarativeListProperty*)&property)->append,
            ((QDeclarativeListProperty*)&property)->count,
            ((QDeclarativeListProperty*)&property)->at,
            ((QDeclarativeListProperty*)&property)->clear
        ),
    m_copyFunction(m_copyFunction),
    m_deleteFunction(m_deleteFunction) {
    if(m_copyFunction){
        this->data = m_copyFunction(property.data);
    }else{
        this->data = property.data;
    }
}
QtjambiDeclarativeListProperty::QtjambiDeclarativeListProperty(QObject *o, QList<QObject*> &list) :
    QDeclarativeListProperty(o, list),
    m_copyFunction(0),
    m_deleteFunction(0){
}

QtjambiDeclarativeListProperty::QtjambiDeclarativeListProperty(QObject *o, void* d, CopyFunction g, DeleteFunction f, AppendFunction a, CountFunction c, AtFunction t, ClearFunction r ) :
    QDeclarativeListProperty(o, 0, a, c, t, r),
    m_copyFunction(g),
    m_deleteFunction(f){
    if(m_copyFunction){
        this->data = m_copyFunction(d);
    }
}

QObject * QtjambiDeclarativeListProperty::object(){
    return QDeclarativeListProperty::object;
}

void QtjambiDeclarativeListProperty::append(QObject* object){
    if(canAppend()){
        QDeclarativeListProperty::append(this, object);
    }
}

int QtjambiDeclarativeListProperty::count(){
    if(canCount()){
        return QDeclarativeListProperty::count(this);
    }else{
        return -1;
    }
}

QObject* QtjambiDeclarativeListProperty::at(int index){
    if(canAt()){
        return QDeclarativeListProperty::at(this, index);
    }else{
        return Q_NULLPTR;
    }
}

void QtjambiDeclarativeListProperty::clear(){
    if(canClear()){
        return QDeclarativeListProperty::clear(this);
    }
}

bool QtjambiDeclarativeListProperty::canAppend(){
    return QDeclarativeListProperty::append!=0;
}

bool QtjambiDeclarativeListProperty::canAt(){
    return QDeclarativeListProperty::at!=0;
}

bool QtjambiDeclarativeListProperty::canClear(){
    return QDeclarativeListProperty::clear!=0;
}

bool QtjambiDeclarativeListProperty::canCount(){
    return QDeclarativeListProperty::count!=0;
}

