/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#include "customtypes.h"

#include "metajava.h"

#include <QtCore/QDebug>
#include <QtCore/QTextStream>


void QModelIndexTypeEntry::generateCppJavaToQt(QTextStream &s,
        const AbstractMetaType *,
        const QString &env_name,
        const QString &qt_name,
        const QString &java_name) const {
    s << "QModelIndex " << qt_name << " = qtjambi_to_QModelIndex(" << env_name << ", "
    << java_name << ")";
}


void QModelIndexTypeEntry::generateCppQtToJava(QTextStream &s,
        const AbstractMetaType *,
        const QString &env_name,
        const QString &qt_name,
        const QString &java_name) const {
    s << "jobject " << java_name << " = qtjambi_from_QModelIndex(" << env_name << ", "
    << qt_name << ")";
}
