/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "docnamespace.h"

DocNamespace::DocNamespace(QObject* parent):
    DocElement(parent)
{

}

void DocNamespace::addFunction(const DocFunction* fct){
    Q_ASSERT(fct);
    for(const DocFunction* f : m_functions.values(fct->name())){
        if(f->isConst()==fct->isConst()
                && f->isStatic()==fct->isStatic()
                && f->type()==fct->type()
                && f->parameters()==fct->parameters()){
            delete fct;
            return;
        }
    }
    m_functions.insert(fct->name(), fct);
}

QList<const DocFunction*> DocNamespace::getFunctions(const QString& name) const{
    return m_functions.values(name);
}
