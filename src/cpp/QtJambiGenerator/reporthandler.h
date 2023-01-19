/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of QtJambi.
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

#ifndef REPORTHANDLER_H
#define REPORTHANDLER_H

#include <QtCore/QReadWriteLock>
#include <QtCore/QThreadStorage>
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

        static void setContext(const QString &context) { m_context.setLocalData(context); }

        static DebugLevel debugLevel() { return m_debug_level; }
        static void setDebugLevel(DebugLevel level) { m_debug_level = level; }

        static int suppressedCount() { return m_suppressed_count; }

        static void warning(const QString &str);

        static void error(const QString &str);

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

        static const QStringList& reportedWarnings();
    private:
        static QReadWriteLock m_lock;
        static int m_suppressed_count;
        static DebugLevel m_debug_level;
        static QThreadStorage<QString> m_context;
        static QStringList m_reported_warnings;
};

#endif // REPORTHANDLER_H
