/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#ifndef DOCPARSER_H
#define DOCPARSER_H

#include <QtCore/QString>


class AbstractMetaClass;
class AbstractMetaFunction;
class AbstractMetaEnum;
class AbstractMetaEnumValue;
class QDomDocument;

class DocParser {
    public:
        DocParser(const QString &docFile);
        ~DocParser();

        QString documentation(const AbstractMetaClass *meta_class) const;
        QString documentation(const AbstractMetaEnum *meta_enum) const;
        QString documentation(const AbstractMetaEnumValue *meta_enum) const;
        QString documentationForFunction(const QString &signature) const;
        QString documentationForSignal(const QString &signature) const;

    private:
        QString documentationForFunction(const QString &signature, const QString &tag) const;
        void build();

        QString m_doc_file;
        QDomDocument *m_dom;
};

#endif // DOCPARSER_H
