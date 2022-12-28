/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of QtJambi.
**
** $BEGIN_LICENSE$
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

#include <QMutexLocker>
#include "reporthandler.h"
#include "typesystem/typedatabase.h"

using namespace TS;

QReadWriteLock ReportHandler::m_lock;
int ReportHandler::m_suppressed_count = 0;
QThreadStorage<QString> ReportHandler::m_context;
ReportHandler::DebugLevel ReportHandler::m_debug_level = NoDebug;
QStringList ReportHandler::m_reported_warnings;

const QStringList& ReportHandler::reportedWarnings(){
    QReadLocker locker(&m_lock);
    return m_reported_warnings;
}

void ReportHandler::warning(const QString &text) {
    QWriteLocker locker(&m_lock);
    QString warningText = QString("WARNING(%1) :: %2").arg(m_context.localData(), text);

    TypeDatabase *db = TypeDatabase::instance();
    if (db && db->isSuppressedWarning(warningText)) {
        ++m_suppressed_count;
    } else if (!m_reported_warnings.contains(warningText)) {
        qDebug("%s", qPrintable(warningText));
        m_reported_warnings.append(warningText);
    }
}

void ReportHandler::error(const QString &text) {
    QWriteLocker locker(&m_lock);
    QString warningText = QString("ERROR(%1) :: %2").arg(m_context.localData(), text);

    TypeDatabase *db = TypeDatabase::instance();
    if (db && db->isSuppressedWarning(warningText)) {
        ++m_suppressed_count;
    } else if (!m_reported_warnings.contains(warningText)) {
        qDebug("%s", qPrintable(warningText));
        m_reported_warnings.append(warningText);
    }
}

void ReportHandler::debug(DebugLevel level, const QString &text) {
    if (m_debug_level == NoDebug)
        return;

    if (level <= m_debug_level)
        qDebug("DEBUG(%s) :: %s", qPrintable(m_context.localData()), qPrintable(text));
}
