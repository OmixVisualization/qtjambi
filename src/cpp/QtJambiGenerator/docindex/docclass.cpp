/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "docclass.h"

DocClass::DocClass(QObject* parent):DocElement(parent),
    m_variables(),
    m_properties(),
    m_functions()
{

}

void DocClass::addFunction(const DocFunction* fct){
    Q_ASSERT(fct);
    m_functions.insert(fct->name(), fct);
}

void DocClass::addVariable(const DocVariable* var){
    Q_ASSERT(var);
    m_variables[var->name()] = var;
}

void DocClass::addProperty(const DocProperty* prop){
    Q_ASSERT(prop);
    m_properties[prop->name()] = prop;
}

void DocClass::addTypeDef(const DocTypeDef* def){
    Q_ASSERT(def);
    m_typeDefs[def->name()] = def;
}

QList<const DocFunction*> DocClass::getFunctions(const QString& name) const{
    return m_functions.values(name);
}

const DocVariable* DocClass::getVariable(const QString& name) const{
    return m_variables[name];
}

const DocProperty* DocClass::getProperty(const QString& name) const{
    return m_properties[name];
}

const DocTypeDef* DocClass::getTypeDef(const QString& name) const{
    return m_typeDefs[name];
}

