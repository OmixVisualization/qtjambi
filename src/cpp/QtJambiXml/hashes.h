/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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

#ifndef QTJAMBIXML_HASHES_H
#define QTJAMBIXML_HASHES_H

#include <QtXml/QtXml>
#include <QtJambi/Global>

inline size_t qHash(const QDomNamedNodeMap &value, size_t seed = 0);

inline size_t qHash(const QDomNode &value, size_t seed = 0)
{
    if(value.parentNode().isNull())
        return 0;
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, int(value.nodeType()));
    seed = hash(seed, value.nodeName());
    seed = hash(seed, value.nodeValue());
    seed = hash(seed, value.prefix());
    seed = hash(seed, value.attributes());
    seed = hash(seed, value.parentNode());
    return seed;
}

inline size_t qHash(const QDomNamedNodeMap &value, size_t seed)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombineCommutative hash;
#else
    QtPrivate::QHashCombineCommutative hash(seed);
#endif
    seed = hash(seed, value.count());
    for (int i=0; i< value.count(); ++i) {
        seed = hash(seed, value.item(i));
    }
    return seed;
}

inline size_t qHash(const QDomNodeList &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombineCommutative hash;
#else
    QtPrivate::QHashCombineCommutative hash(seed);
#endif
    seed = hash(seed, value.count());
    for (int i=0; i< value.count(); ++i) {
        seed = hash(seed, value.item(i));
    }
    return seed;
}

inline size_t qHash(const QDomImplementation &impl, size_t seed = 0)
{
    return qHash(*reinterpret_cast<void*const*>(&impl), seed);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
inline bool operator==(const QDomDocument::ParseResult &value1, const QDomDocument::ParseResult &value2){
    return value1.errorColumn==value2.errorColumn && value1.errorLine==value2.errorLine && value1.errorMessage==value2.errorMessage;
}

inline size_t qHash(const QDomDocument::ParseResult &value, size_t seed = 0)
{
    return qHashMulti(seed, value.errorColumn, value.errorLine, value.errorMessage);
}
#endif

#endif // QTJAMBIXML_HASHES_H
