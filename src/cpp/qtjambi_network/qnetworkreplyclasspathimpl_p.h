/****************************************************************************
**
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QNETWORKREPLYCLASSPATHIMPL_P_H
#define QNETWORKREPLYCLASSPATHIMPL_P_H

#include <QtCore>
#include <QtNetwork>

class QNetworkReplyClassPathImpl : public QNetworkReply{

private: QBuffer m_buffer;

public:
    explicit QNetworkReplyClassPathImpl(QNetworkAccessManager::Operation op, const QByteArray& data, const QNetworkRequest& request, QObject* parent = Q_NULLPTR);

    virtual void abort() Q_DECL_OVERRIDE;

    virtual qint64 readData(char *data, qint64 len) Q_DECL_OVERRIDE;

    virtual qint64 size() const Q_DECL_OVERRIDE;

    virtual qint64 pos() const Q_DECL_OVERRIDE;

    virtual qint64 bytesAvailable() const Q_DECL_OVERRIDE;

    virtual bool isSequential() const Q_DECL_OVERRIDE;
};

#endif // QNETWORKREPLYCLASSPATHIMPL_P_H

