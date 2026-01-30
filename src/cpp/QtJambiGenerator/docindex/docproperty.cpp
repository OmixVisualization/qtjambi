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

#include "docproperty.h"

DocProperty::DocProperty(QObject* parent) : DocElement(parent)
{

}

const QString& DocProperty::name() const {
    return m_name;
}

const QString& DocProperty::type() const {
    return m_type;
}

const QString& DocProperty::setter() const {
    return m_setter;
}

const QString& DocProperty::getter() const {
    return m_getter;
}

void DocProperty::setName(const QString& name) {
    m_name = name;
}

void DocProperty::setType(const QString& type) {
    m_type = type;
}

void DocProperty::setSetter(const QString& setter) {
    m_setter = setter;
}

void DocProperty::setGetter(const QString& getter) {
    m_getter = getter;
}
