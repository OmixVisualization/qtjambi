/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "docmodel.h"

DocModel::DocModel(): QObject(),
    m_namespaces() {}

bool DocModel::isEmpty() const{
    return m_namespaces.isEmpty();
}

void DocModel::addNamespace(DocNamespace* ns){
    Q_ASSERT(ns);
    m_namespaces.insert(ns->fullName(), ns);
}

DocNamespace* DocModel::getNamespace(const QString& name){
    return m_namespaces[name];
}

const DocNamespace* DocModel::getNamespace(const QString& name) const{
    return m_namespaces[name];
}

QList<QString> DocModel::namespaces() const {
    return m_namespaces.keys();
}

void DocModel::addClass(const DocClass* cls){
    Q_ASSERT(cls);
    m_classes.insert(cls->fullName(), cls);
}

void DocModel::addEnum(const DocEnum* enm){
    Q_ASSERT(enm);
    m_enums[enm->fullName()] = enm;
}

const DocClass* DocModel::getClass(const QString& name) const{
    return m_classes[name];
}

const DocEnum* DocModel::getEnum(const QString& name) const{
    return m_enums[name];
}

QList<QString> DocModel::classes() const{
    return m_classes.keys();
}

const QString& DocModel::url() const{
    return m_url;
}

void DocModel::setUrl(const QString& url){
    m_url = url;
}
