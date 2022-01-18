/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_SCRIPT_HASHES_H
#define QTJAMBI_SCRIPT_HASHES_H


#include <QtScript/QtScript>


inline hash_type qHash(const QScriptContextInfo &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.scriptId());
    hashCode = hashCode * 31 + qHash(value.fileName());
    hashCode = hashCode * 31 + qHash(value.lineNumber());
    hashCode = hashCode * 31 + qHash(value.columnNumber());
    hashCode = hashCode * 31 + qHash(value.functionName());
    hashCode = hashCode * 31 + qHash(value.functionType());
    hashCode = hashCode * 31 + qHash(value.functionParameterNames());
    hashCode = hashCode * 31 + qHash(value.functionStartLineNumber());
    hashCode = hashCode * 31 + qHash(value.functionEndLineNumber());
    hashCode = hashCode * 31 + qHash(value.functionMetaIndex());
    return hashCode;
}

inline hash_type qHash(const QScriptProgram &value)
{
    hash_type hashCode = 1;
    hashCode = hashCode * 31 + qHash(value.fileName());
    hashCode = hashCode * 31 + qHash(value.firstLineNumber());
    hashCode = hashCode * 31 + qHash(value.sourceCode());
    return hashCode;
}

#endif // QTJAMBI_SCRIPT_HASHES_H
