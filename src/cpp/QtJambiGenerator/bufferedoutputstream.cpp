/****************************************************************************
**
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

#include "bufferedoutputstream.h"
#include "reporthandler.h"

#include <QFileInfo>
#include <QDir>

BufferedOutputStream::BufferedOutputStream(const QFileInfo& file):
        m_file(file),
        m_buffer() {
    QBuffer* buffer = new QBuffer(&m_buffer);
    if(buffer->open(QIODevice::WriteOnly))
        setDevice(buffer);
}

BufferedOutputStream::~BufferedOutputStream() {
    if (device())
        finish();
}

bool BufferedOutputStream::finish() {
    if(!device())
        return true;
    flush();
    {
        QBuffer* buffer = static_cast<QBuffer*>(device());
        buffer->close();
        setDevice(nullptr);
        delete buffer;
    }
    bool fileEqual = false;
    QFile fileRead(m_file.absoluteFilePath());
    QFileInfo info(fileRead);
    QByteArray original;
    if (info.exists() && info.size() == m_buffer.size()) {
        if (!fileRead.open(QIODevice::ReadOnly)) {
            ReportHandler::warning(QString("failed to open file '%1' for reading")
                                   .arg(fileRead.fileName()));
            return false;
        }

        original = fileRead.readAll();
        fileRead.close();
        fileEqual = (original == m_buffer);
    }

    if (!fileEqual) {
        QDir dir(info.absolutePath());
        if (!dir.mkpath(dir.absolutePath())) {
            ReportHandler::warning(QString("unable to create directory '%1'")
                                   .arg(dir.absolutePath()));
            return false;
        }

        QFile fileWrite(m_file.absoluteFilePath());
        if (!fileWrite.open(QIODevice::WriteOnly)) {
            ReportHandler::warning(QString("failed to open file '%1' for writing")
                                   .arg(fileWrite.fileName()));
            return false;
        }
        fileWrite.write(m_buffer);
        fileWrite.close();
        return true;
    }
    return false;
}
