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

#include "qtjambifunctiontable.h"
#include "qtjambi_cache.h"

#include <memory.h>

QtJambiFunctionTable::QtJambiFunctionTable(const QString &className,
                                         int size)
    : m_class_name(className),
      m_method_count(size),
      m_reference_count(1)
{
    m_method_ids = new jmethodID[size];

    for (int i=0; i<size; ++i)
        m_method_ids[i] = 0;
}

QtJambiFunctionTable::~QtJambiFunctionTable()
{
    removeFunctionTable(this);
    delete [] m_method_ids;
}

void QtJambiFunctionTable::ref()
{
    mutex.lock();
    ++m_reference_count;
    Q_ASSERT(m_reference_count > 0);
    mutex.unlock();
}

void QtJambiFunctionTable::deref()
{
    mutex.lock();
    --m_reference_count;
    Q_ASSERT(m_reference_count >= 0);
    mutex.unlock();
}
