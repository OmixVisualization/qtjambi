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

#ifndef QDOCMODEL_H
#define QDOCMODEL_H

#include <QObject>
#include "docclass.h"
#include "docnamespace.h"

class DocNamespace;
class DocEnum;
class DocClass;

class DocModel: public QObject
{
public:
    DocModel();
    void addNamespace(DocNamespace*);
    DocNamespace* getNamespace(const QString& name);
    const DocNamespace* getNamespace(const QString& name) const;
    QList<QString> namespaces() const;
    bool isEmpty() const;
    void addClass(const DocClass*);
    void addEnum(const DocEnum*);
    const DocClass* getClass(const QString& name) const;
    const DocEnum* getEnum(const QString& name) const;
    QList<QString> classes() const;
    const QString& url() const;
    void setUrl(const QString& url);
private:
    QString m_url;
    QMap<QString,DocNamespace*> m_namespaces;
    QMap<QString,const DocEnum*> m_enums;
    QMap<QString,const DocClass*> m_classes;
};

#endif // QDOCMODEL_H
