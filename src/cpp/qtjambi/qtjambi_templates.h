/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#ifndef QTJAMBI_TEMPLATES_H
#define QTJAMBI_TEMPLATES_H

#include <QtCore/QDataStream>
#include "qtjambi_global.h"

template <typename T>
static void qtjambiGenericDeleter(void* ptr)
{
    T *t = reinterpret_cast<T*>(ptr);
    delete t;
}

template <typename T>
static void qtjambiGenericDestructor(void *t)
{
    static_cast<T*>(t)->~T();
}

template <typename T>
static void *qtjambiGenericConstructor(void *where, const void *t)
{
    if (t)
        return new (where) T(*static_cast<const T*>(t));
    return new (where) T;
}

template <typename T>
static void *qtjambiGenericCreator(const void *ptr)
{
    if (ptr)
        return new T(*static_cast<const T *>(ptr));
    return new T();
}

#ifndef QT_NO_DATASTREAM
template <typename T>
static void qGenericSaveEnumerator(QDataStream &stream, const void *t)
{
    switch(sizeof(T)){
    case 1: stream << *static_cast<const quint8*>(t); break;
    case 2: stream << *static_cast<const quint16*>(t); break;
    case 4: stream << *static_cast<const quint32*>(t); break;
    case 8: stream << *static_cast<const quint64*>(t); break;
    default: stream << *static_cast<const quint32*>(t); break;
    }
}

template <typename T>
static void qGenericLoadEnumerator(QDataStream &stream, void *t)
{
    switch(sizeof(T)){
    case 1: stream >> *static_cast<quint8*>(t); break;
    case 2: stream >> *static_cast<quint16*>(t); break;
    case 4: stream >> *static_cast<quint32*>(t); break;
    case 8: stream >> *static_cast<quint64*>(t); break;
    default: stream >> *static_cast<quint32*>(t); break;
    }
}
#endif // QT_NO_DATASTREAM

#endif // QTJAMBI_TEMPLATES_H
