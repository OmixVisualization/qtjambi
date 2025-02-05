/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of QtJambi.
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


#ifndef ASTTOXML
#define ASTTOXML

#include "codemodel.h"

#include <QString>
#include <QXmlStreamWriter>

void astToXML(const QString& name, FileModelItem dom);
void writeOutNamespace(QXmlStreamWriter &s, const NamespaceModelItem &item);
void writeOutEnum(QXmlStreamWriter &s, const EnumModelItem &item);
void writeOutFunction(QXmlStreamWriter &s, const FunctionModelItem &item);
void writeOutClass(QXmlStreamWriter &s, const ClassModelItem &item);
void writeOutProperty(QXmlStreamWriter &, const QString &);
void writeHelper(QXmlStreamWriter &, const QLatin1String&, const QString &);
void writeType(QXmlStreamWriter &, const TypeInfo &);


#endif // ASTTOXML
