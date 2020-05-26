/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTJAMBI_XML_HASHES_H
#define QTJAMBI_XML_HASHES_H

#include <QtXml/QtXml>

inline uint qHash(const QDomNamedNodeMap &value);

inline uint qHash(const QDomNode &value)
{
    if(value.parentNode().isNull())
        return 0;
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(value.nodeType()));
    hashCode = hashCode * 31 + qHash(value.nodeName());
    hashCode = hashCode * 31 + qHash(value.nodeValue());
    hashCode = hashCode * 31 + qHash(value.prefix());
    hashCode = hashCode * 31 + qHash(value.attributes());
    hashCode = hashCode * 31 + qHash(value.parentNode());
    return hashCode;
}

inline uint qHash(const QDomNamedNodeMap &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.count());
    for (int i=0; i< value.count(); ++i) {
        hashCode = hashCode * 31 + qHash(value.item(i));
    }
    return hashCode;
}

inline uint qHash(const QDomNodeList &value)
{
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.count());
    for (int i=0; i< value.count(); ++i) {
        hashCode = hashCode * 31 + qHash(value.item(i));
    }
    return hashCode;
}

inline uint qHash(const QDomImplementation &)
{
    return 0;
}

#endif // QTJAMBI_XML_HASHES_H
