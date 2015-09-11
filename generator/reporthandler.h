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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef REPORTHANDLER_H
#define REPORTHANDLER_H

#include <QtCore/QString>
#include <QtCore/QSet>

class ReportHandler {
    public:
        enum DebugLevel {
            NoDebug,
            TypeDebug,
            SparseDebug,
            MediumDebug,
            FullDebug
        };

        static void setContext(const QString &context) { m_context = context; }

        static DebugLevel debugLevel() { return m_debug_level; }
        static void setDebugLevel(DebugLevel level) { m_debug_level = level; }

        static int warningCount() { return m_warning_count; }

        static int suppressedCount() { return m_suppressed_count; }

        static void warning(const QString &str);

        static void debugSparse(const QString &str) {
            debug(SparseDebug, str);
        }

        static void debugTypes(const QString &str) {
            debug(TypeDebug, str);
        }

        static void debugMedium(const QString &str) {
            debug(MediumDebug, str);
        }

        static void debugFull(const QString &str) {
            debug(FullDebug, str);
        }

        static void debug(DebugLevel level, const QString &str);

    private:
        static int m_warning_count;
        static int m_suppressed_count;
        static DebugLevel m_debug_level;
        static QString m_context;
        static QSet<QString> m_reported_warnings;
};

#endif // REPORTHANDLER_H
