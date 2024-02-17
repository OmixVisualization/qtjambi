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

#ifndef DOCCLASS_H
#define DOCCLASS_H

#include "docelement.h"
#include "docenum.h"
#include "docfunction.h"
#include "docvariable.h"
#include "docproperty.h"
#include "doctypedef.h"


class DocClass: public DocElement
{
public:
    DocClass(QObject* parent);

    void addFunction(const DocFunction*);
    void addVariable(const DocVariable*);
    void addProperty(const DocProperty*);
    void addTypeDef(const DocTypeDef*);
    QList<const DocFunction*> getFunctions(const QString& name) const;
    const DocVariable* getVariable(const QString& name) const;
    const DocProperty* getProperty(const QString& name) const;
    const DocTypeDef* getTypeDef(const QString& name) const;
private:
    QMap<QString,const DocVariable*> m_variables;
    QMap<QString,const DocProperty*> m_properties;
    QMap<QString,const DocTypeDef*> m_typeDefs;
    QMultiHash<QString,const DocFunction*> m_functions;
};

#endif // DOCCLASS_H
