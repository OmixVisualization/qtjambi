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

#ifndef QTJAMBIQML_HASHES_H
#define QTJAMBIQML_HASHES_H

#include <QtCore/QtGlobal>
#include <QtQml/QQmlScriptString>
#include <QtQml/QQmlError>
#include <QtQml/QQmlListReference>
#include <QtQml/QJSEngine>
#include <QtJambi/QtJambiAPI>

#if QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)
#include <QtQml/QJSPrimitiveValue>

inline hash_type qHash(const QJSPrimitiveValue &value, hash_type seed = 0)
{
    switch(value.type()){
    case QJSPrimitiveValue::Undefined:
        return std::numeric_limits<hash_type>::max();
    case QJSPrimitiveValue::Null:
        return 0;
    case QJSPrimitiveValue::Boolean:
        return qHash(value.toBoolean(), seed);
    case QJSPrimitiveValue::Integer:
        return qHash(value.toInteger(), seed);
    case QJSPrimitiveValue::Double:
        return qHash(value.toDouble(), seed);
    case QJSPrimitiveValue::String:
        return qHash(value.toString(), seed);
    }
    return 0;
}
#endif // QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)

inline hash_type qHash(const QQmlScriptString &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.isEmpty());
    seed = hash(seed, value.isNullLiteral());
    seed = hash(seed, value.isUndefinedLiteral());
    seed = hash(seed, value.stringLiteral());
    bool ok = false;
    seed = hash(seed, value.numberLiteral(&ok));
    seed = hash(seed, ok);
    ok = false;
    seed = hash(seed, value.booleanLiteral(&ok));
    seed = hash(seed, ok);
    return seed;
}

inline hash_type qHash(const QQmlError &value, hash_type seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.url());
    seed = hash(seed, value.description());
    seed = hash(seed, value.line());
    seed = hash(seed, value.column());
    seed = hash(seed, quintptr(value.object()));
    seed = hash(seed, value.messageType());
    return seed;
}

hash_type qHash(const QQmlListReference &value, hash_type seed = 0);

namespace QtQml {
    typedef QObject *(*QQmlAttachedPropertiesFunc)(QObject *);
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
    typedef std::function<QObject*(QQmlEngine *, QJSEngine *)> ObjectCallback;
#else
    typedef QObject *(*ObjectCallback)(QQmlEngine *, QJSEngine *);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    typedef std::function<QJSValue(QQmlEngine *, QJSEngine *)> ValueCallback;
#else
    typedef QJSValue (*ValueCallback)(QQmlEngine *, QJSEngine *);
#endif
}

#if defined(QTJAMBI_GENERATOR_RUNNING) && QT_VERSION >= QT_VERSION_CHECK(6,7,0)
template <typename T>
int qmlRegisterSingletonType(
    const char *uri, int versionMajor, int versionMinor,  const char *typeName,
    QObject*(*callback)(QQmlEngine*,QJSEngine*));
int qmlRegisterSingletonType(
    const char *uri, int versionMajor, int versionMinor, const char *typeName,
    QJSValue(*callback)(QQmlEngine *, QJSEngine *))
#endif

#endif // QTJAMBIQML_HASHES_H
