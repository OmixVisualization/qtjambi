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

#include "docelement.h"

DocElement::DocElement(QObject* parent)
    : QObject(parent),
      m_name(),
      m_fullName(),
      m_href(),
      m_brief(),
      m_subdir()
{

}

const QString& DocElement::name() const{
    return m_name;
}

const QString& DocElement::url() const{
    return m_url;
}

const QString& DocElement::fullName() const{
    return m_fullName.isEmpty() ? m_name : m_fullName;
}

const QString& DocElement::href() const{
    return m_href;
}

const QString& DocElement::brief() const{
    return m_brief;
}

const QDir& DocElement::subdir() const{
    return m_subdir;
}

void DocElement::setName(const QString& name){
    m_name = name;
}

void DocElement::setUrl(const QString& url){
    m_url = url;
}

void DocElement::setFullName(const QString& fullName){
    m_fullName = fullName;
}

void DocElement::setHref(const QString& href){
    m_href = href;
}

void DocElement::setBrief(const QString& brief){
    m_brief = brief;
}

void DocElement::setSubdir(const QDir& subdir){
    m_subdir = subdir;
}

const QString& DocElement::since() const
{
    return m_since;
}

void DocElement::setSince(const QString &newSince)
{
    m_since = newSince;
}
