/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "javautils.h"
#include <QtCore/QHash>
#include <cstdio>

QString javaFixString(const QString &str)
{
    const ushort *chars = str.utf16();
    QString result;

    for (int i = 0; i < str.length(); ++i) {
        ushort c = chars[i];
        if (c >= 0x0080) {
            QString num = QString::number(c, 16);
            int padding = 4 - num.length();
            if (padding < 0) {
                qWarning("juic: bad unicode character %x became %s\n", c, qPrintable(num));
                return "";
            }

            result += QLatin1String("\\u") + QString(padding, '0') + QString::number(c, 16);
        } else {
            switch(c) {
            case '\\':
                result += QLatin1String("\\\\"); break;
            case '\"':
                result += QLatin1String("\\\""); break;
            case '\r':
                break;
            case '\n':
                result += QLatin1String("\\n\"+\n\""); break;
            default:
                result += QChar(c);
            }
        }
    }

    return QLatin1String("\"") + result + QLatin1String("\"");
}


QString escapeVariableName(const QString &name)
{
    static QHash<QString, bool> escaped_names;
    if (escaped_names.isEmpty()) {
        escaped_names[QLatin1String("native")] = false;
        escaped_names[QLatin1String("boolean")] = false;
        escaped_names[QLatin1String("abstract")] = false;
        escaped_names[QLatin1String("final")] = false;
    }

    if (escaped_names.contains(name)) {
        bool &reported = escaped_names[name];

        if (!reported) {
            fprintf(stderr, "juic: Variable '%s' renamed to '%s__'\n", qPrintable(name), qPrintable(name));
            reported = true;
        }

        return name + QLatin1String("__");
    }
    return name;
}
