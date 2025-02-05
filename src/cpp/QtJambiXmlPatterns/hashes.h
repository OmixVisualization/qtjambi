/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBIXMLPATTERNS_HASHES_H
#define QTJAMBIXMLPATTERNS_HASHES_H

#include <QtXmlPatterns/QtXmlPatterns>
#include <QtJambi/Global>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline bool operator==(const QXmlItem& arg1, const QXmlItem& arg2){
    if(arg1.isNull()==arg2.isNull())
        return true;
    if(arg1.isNode()==arg2.isNode())
        return arg1.toNodeModelIndex()==arg2.toNodeModelIndex();
    if(arg1.isAtomicValue()==arg2.isAtomicValue())
        return arg1.toAtomicValue()==arg2.toAtomicValue();
    return false;
}

inline hash_type qHash(const QXmlItem &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.isNull());
    seed = hash(seed, value.isNode());
    seed = hash(seed, value.isAtomicValue());
    if(value.isNode())
        seed = hash(seed, value.toNodeModelIndex());
    if(value.isAtomicValue())
        seed = hash(seed, value.toAtomicValue());
    return seed;
}

inline bool operator==(const QXmlNamePool& arg1, const QXmlNamePool& arg2){
    return reinterpret_cast<const QExplicitlySharedDataPointer<char>&>(arg1).constData() == reinterpret_cast<const QExplicitlySharedDataPointer<char>&>(arg2).constData();
}

inline hash_type qHash(const QXmlNamePool &value, hash_type seed = 0)
{
    return qHash(qintptr(reinterpret_cast<const QExplicitlySharedDataPointer<char>&>(value).constData()), seed);
}

inline bool operator==(const QXmlSchema& arg1, const QXmlSchema& arg2){
    return arg1.isValid()==arg2.isValid()
            && arg1.namePool()==arg2.namePool()
            && arg1.documentUri()==arg2.documentUri()
            && arg1.messageHandler()==arg2.messageHandler()
            && arg1.uriResolver()==arg2.uriResolver()
            && arg1.networkAccessManager()==arg2.networkAccessManager();
}

inline hash_type qHash(const QXmlSchema &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.isValid());
    seed = hash(seed, value.namePool());
    seed = hash(seed, value.documentUri());
    seed = hash(seed, value.messageHandler());
    seed = hash(seed, value.uriResolver());
    seed = hash(seed, value.networkAccessManager());
    return seed;
}
#endif

#endif // QTJAMBIXMLPATTERNS_HASHES_H
