/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

inline hash_type qHash(const QDomNamedNodeMap &value, hash_type seed = 0);

inline hash_type qHash(const QDomNode &value, hash_type seed = 0)
{
    if(value.parentNode().isNull())
        return 0;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(value.nodeType()));
    seed = hash(seed, value.nodeName());
    seed = hash(seed, value.nodeValue());
    seed = hash(seed, value.prefix());
    seed = hash(seed, value.attributes());
    seed = hash(seed, value.parentNode());
    return seed;
}

inline hash_type qHash(const QDomNamedNodeMap &value, hash_type seed)
{
    QtPrivate::QHashCombineCommutative hash;
    seed = hash(seed, value.count());
    for (int i=0; i< value.count(); ++i) {
        seed = hash(seed, value.item(i));
    }
    return seed;
}

inline hash_type qHash(const QDomNodeList &value, hash_type seed = 0)
{
    QtPrivate::QHashCombineCommutative hash;
    seed = hash(seed, value.count());
    for (int i=0; i< value.count(); ++i) {
        seed = hash(seed, value.item(i));
    }
    return seed;
}

inline hash_type qHash(const QDomImplementation &impl, hash_type seed = 0)
{
    return qHash(*reinterpret_cast<void*const*>(&impl), seed);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline QT_DEPRECATED_VERSION(5, 15) bool operator==(const QXmlParseException& arg1, const QXmlParseException& arg2){
    return arg1.columnNumber()==arg2.columnNumber()
            && arg1.lineNumber()==arg2.lineNumber()
            && arg1.publicId()==arg2.publicId()
            && arg1.systemId()==arg2.systemId()
            && arg1.message()==arg2.message();
}

inline QT_DEPRECATED_VERSION(5, 15) hash_type qHash(const QXmlParseException &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.columnNumber());
    seed = hash(seed, value.lineNumber());
    seed = hash(seed, value.publicId());
    seed = hash(seed, value.systemId());
    seed = hash(seed, value.message());
    return seed;
}

inline bool operator==(const QXmlAttributes& arg1, const QXmlAttributes& arg2){
    if(arg1.count()!=arg2.count())
        return false;
    for (int i=0; i< arg1.count(); ++i) {
        if(arg1.localName(i)!=arg2.localName(i)
                || arg1.qName(i)!=arg2.qName(i)
                || arg1.uri(i)!=arg2.uri(i)
                || arg1.type(i)!=arg2.type(i)
                || arg1.value(i)!=arg2.value(i)){
            return false;
        }
    }
    return true;
}

inline hash_type qHash(const QXmlAttributes &value, hash_type seed = 0)
{
    QtPrivate::QHashCombineCommutative hash;
    seed = hash(seed, value.count());
    for (int i=0; i< value.count(); ++i) {
        seed = hash(seed, value.localName(i));
        seed = hash(seed, value.qName(i));
        seed = hash(seed, value.uri(i));
        seed = hash(seed, value.type(i));
        seed = hash(seed, value.value(i));
    }
    return seed;
}
#endif

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
