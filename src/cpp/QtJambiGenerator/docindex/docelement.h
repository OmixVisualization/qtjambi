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

#ifndef DOCELEMENT_H
#define DOCELEMENT_H

#include <QtCore>

class DocElement: public QObject
{
public:
    DocElement(QObject* parent);

    const QString& name() const;
    const QString& url() const;
    const QString& fullName() const;
    const QString& href() const;
    const QString& brief() const;
    const QDir& subdir() const;
    void setName(const QString& name);
    void setUrl(const QString& url);
    void setFullName(const QString& fullName);
    void setHref(const QString& href);
    void setBrief(const QString& brief);
    void setSubdir(const QDir& subdir);
private:
    QString m_url;
    QString m_name;
    QString m_fullName;
    QString m_href;
    QString m_brief;
    QDir m_subdir;
};

#endif // DOCELEMENT_H
