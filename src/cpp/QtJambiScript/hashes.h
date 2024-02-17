/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBISCRIPT_HASHES_H
#define QTJAMBISCRIPT_HASHES_H

#include <QtJambi/Global>
#include <QtScript/QScriptContextInfo>
#include <QtScript/QScriptProgram>

inline hash_type qHash(const QScriptContextInfo &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.scriptId());
    seed = hash(seed, value.fileName());
    seed = hash(seed, value.lineNumber());
    seed = hash(seed, value.columnNumber());
    seed = hash(seed, value.functionName());
    seed = hash(seed, value.functionType());
    seed = hash(seed, value.functionParameterNames());
    seed = hash(seed, value.functionStartLineNumber());
    seed = hash(seed, value.functionEndLineNumber());
    seed = hash(seed, value.functionMetaIndex());
    return seed;
}

inline hash_type qHash(const QScriptProgram &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.fileName());
    seed = hash(seed, value.firstLineNumber());
    seed = hash(seed, value.sourceCode());
    return seed;
}

#endif // QTJAMBISCRIPT_HASHES_H
